#include <malloc.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <mem.h>
#include <stdio.h>

#include "Vector.h"
#include "Tracer.h"
#include "Targa.h"

void SetMode(int Mode)
{
        asm {
                mov     ax, Mode
                int     10h
        }
}

void SetPalette(RGB far *Palette)
{
        asm {
                push    es
                mov     ax, 1012h
                mov     bx, 0 // first color to set
                mov     cx, 256 // # of colors
                les     dx, Palette // ES:DX == table of color values
                int     10h
                pop     es
        }
}

void SetPreviewPalette()
{
        RGB Pal[256];
        int i;

        for (i = 0; i < 256; i++)
        {
                Pal[i].Red = (63 * (i & 7)) / 7;
                Pal[i].Green = (63 * ((i >> 3) & 7)) / 7;
                Pal[i].Blue = (63 * ((i >> 6) & 3)) / 3;
        }

        SetPalette(Pal);
}

void DrawPixel(int x, int y, Vector &Color)
{
        int r = Color.x * 7 + 0.5;
        int g = Color.y * 7 + 0.5;
        int b = Color.z * 3 + 0.5;

        pokeb(0xA000, x + y * 320, r + (g << 3) + (b << 6));
}

struct ColorData
{
        int Hue;  // color value
        int Freq; // it's frequency
};

int ColorDataComp(const void *v1, const void *v2)
{
        return ((ColorData *)v2)->Freq - ((ColorData *)v1)->Freq;
}

void BuildImagePalette(char far *ColorTrans, RGB *Palette)
{
        ColorData *ColorTable = new ColorData[8192];
        int MinDist;
        int d;
        unsigned i, j;
        int r, g, b;
        int index;
        int ColorsCount;

        if (ColorTable == NULL)
        {
                printf("\nNo memory for ColorTable");
                exit(1);
        }
        // prepare used colors table ( color, frequency )
        for (ColorsCount = 0, i = 0; i < 32768; i++)
                if (ColorTrans[i] > 0 && ColorsCount < 8192)
                {
                        ColorTable[ColorsCount].Hue = i;
                        ColorTable[ColorsCount].Freq = ColorTrans[i];
                        ColorsCount++;
                }
        // sort table on frequency
        qsort(ColorTable, ColorsCount, sizeof(ColorData), ColorDataComp);
        memset(Palette, 0, 256 * 3);

        for (i = 0; i < 256 && i < ColorsCount; i++)
        { // build 5-bit values [0..31]
                Palette[i].Red = 2 * (ColorTable[i].Hue & 0x1F);
                Palette[i].Green = 2 * ((ColorTable[i].Hue >> 5) & 0x1F);
                Palette[i].Blue = 2 * ((ColorTable[i].Hue >> 10) & 0x1F);
        }
        // find darkest color
        for (MinDist = 1024, i = 0; i < 256; i++)
        {
                int d = (int)Palette[i].Red + (int)Palette[i].Green + (int)Palette[i].Blue;

                if (d < MinDist)
                {
                        MinDist = d;
                        index = i;
                }
        }

        if (index != 0) // and make it background
        {
                RGB tmp = Palette[0]; // swap Palette [0] and Palette [index]

                Palette[0] = Palette[index];
                Palette[index] = tmp;
        }

        _fmemset(ColorTrans, 0, 32768);   // init translation to palette color 0
        for (i = 0; i < ColorsCount; i++) // for every used color find closest palette match
        {                                 // get rgb for ColorTable [i]
                r = 2 * (ColorTable[i].Hue & 0x1F);
                g = 2 * ((ColorTable[i].Hue >> 5) & 0x1F);
                b = 2 * ((ColorTable[i].Hue >> 10) & 0x1F);
                // scan palette for closest match
                for (MinDist = 1024, j = 0; j < 256; j++)
                {
                        d = abs(r - Palette[j].Red) + abs(g - Palette[j].Green) + abs(b - Palette[j].Blue);

                        if (d < MinDist)
                        {
                                MinDist = d;
                                index = j;
                        }
                }

                ColorTrans[ColorTable[i].Hue] = index;
        }

        delete ColorTable;
}

void DrawImageFile(char *PicFileName)
{
        int nx, ny;
        int i, j;
        int r, g, b;
        int index;
        double Max;
        RGB Palette[256];
        Vector LineBuffer[800];
        int file = open(PicFileName, O_RDONLY | O_BINARY);
        char far *ColorTrans = (char far *)farmalloc(32768); // color frequencies ( translation ) table

        printf("\nBuilding color histogram");

        if (file == -1)
        {
                printf("\nCannot open %s", PicFileName);
                return;
        }

        if (ColorTrans == NULL)
        {
                printf("\nInsufficient memory for ColorTrans");
                close(file);
                return;
        }

        lseek(file, 0l, SEEK_SET);
        read(file, &nx, 2);
        read(file, &ny, 2);
        read(file, &Max, sizeof(Max));

        _fmemset(ColorTrans, 0, 32768); // init frequencies

        for (i = 0; i < ny; i++)
        {
                read(file, LineBuffer, nx * sizeof(Vector));
                for (j = 0; j < nx; j++)
                {
                        Clip(LineBuffer[j]);

                        r = (int)(LineBuffer[j].x * 31);
                        g = (int)(LineBuffer[j].y * 31);
                        b = (int)(LineBuffer[j].z * 31);
                        index = r | (g << 5) | (b << 10);

                        if (ColorTrans[index] < 255)
                                ColorTrans[index]++;
                }
        }

        BuildImagePalette(ColorTrans, Palette);
        SetMode(0x13);
        SetPalette(Palette);

        lseek(file, 4l + sizeof(Max), SEEK_SET);
        for (i = 0; i < ny; i++)
        {
                read(file, LineBuffer, nx * sizeof(Vector));
                for (j = 0; j < nx; j++)
                {
                        Clip(LineBuffer[j]);

                        r = (int)(LineBuffer[j].x * 31);
                        g = (int)(LineBuffer[j].y * 31);
                        b = (int)(LineBuffer[j].z * 31);
                        index = r | (g << 5) | (b << 10);

                        pokeb(0xA000, j + 320 * i, ColorTrans[index]);
                }
        }

        getch();
        SetMode(0x03);
        close(file);
        farfree(ColorTrans);
}

void DrawTargaFile(char *PicFileName)
{
        int file = open(PicFileName, O_RDONLY | O_BINARY);

        if (file == -1)
        {
                printf("\nCannot open %s", PicFileName);
                return;
        }

        TargaHeader Hdr;
        int r, g, b;
        int index;
        RGB Palette[256];
        RGB *LineBuffer;
        char far *ColorTrans;

        read(file, &Hdr, sizeof(Hdr));       // read header
        lseek(file, Hdr.TextSize, SEEK_CUR); // skip comments

        if (Hdr.DataType != 2)
        {
                printf("\nUnsupported image type.");
                close(file);
                return;
        }
        // allocate space for freq/trans table
        if ((ColorTrans = (char far *)farmalloc(32768)) == NULL)
        {
                printf("\nInsufficient memory for ColorTrans");
                close(file);
                return;
        }

        if ((LineBuffer = new RGB[Hdr.Width]) == NULL)
        {
                printf("\nInsufficient space for Buffer");
                farfree(ColorTrans);
                close(file);
                return;
        }

        _fmemset(ColorTrans, 0, 32768); // init frequencies

        for (int i = 0; i < Hdr.Height; i++)
        {
                read(file, LineBuffer, Hdr.Width * sizeof(RGB));
                for (int j = 0; j < Hdr.Width; j++)
                { // convert to 0..31 range
                        r = LineBuffer[j].Blue >> 3;
                        g = LineBuffer[j].Green >> 3;
                        b = LineBuffer[j].Red >> 3;
                        index = r | (g << 5) | (b << 10);

                        if (ColorTrans[index] < 255)
                                ColorTrans[index]++;
                }
        }

        BuildImagePalette(ColorTrans, Palette);
        SetMode(0x13);
        SetPalette(Palette);

        lseek(file, sizeof(Hdr) + Hdr.TextSize, SEEK_SET);

        for (i = 0; i < Hdr.Height; i++)
        {
                read(file, LineBuffer, Hdr.Width * sizeof(RGB));
                for (int j = 0; j < Hdr.Width; j++)
                {
                        r = LineBuffer[j].Blue >> 3;
                        g = LineBuffer[j].Green >> 3;
                        b = LineBuffer[j].Red >> 3;
                        index = r | (g << 5) | (b << 10);

                        pokeb(0xA000, j + 320 * i, ColorTrans[index]);
                }
        }

        close(file);
        farfree(ColorTrans);
        delete LineBuffer;

        getch();
        SetMode(0x03);
}
