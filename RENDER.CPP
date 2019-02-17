#include <malloc.h>
#include <conio.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include <iostream.h>

#include "Tracer.h"
#include "Draw.h"
#include "Render.h"

long far *TicksPtr = (long far *)0x46CL;

void RenderScene(double HalfWidth, double HalfHeight, int nx, int ny, char *PicFileName)
{
        cout << "\nBegin Tracing...\n\n";
        double x, y;                       // sample point
        double hx = 2.0 * HalfWidth / nx;  // pixel width
        double hy = 2.0 * HalfHeight / ny; // pixel height
        Ray ray;                           // pixel ray
        Vector Color;
        int i, j;
        long Ticks = *TicksPtr;
        TargaFile *tga = new TargaFile(PicFileName, nx, ny);
        RGB c;
        int ProgressionBarStep = (int)(ny / 80);
        if (ProgressionBarStep < 2)
        {
                ProgressionBarStep = 1;
        }

        //       SetMode ( 0x13 );
        //       SetPreviewPalette ();

        for (i = 0, y = HalfHeight; i < ny; i++, y -= hy)
        {
                for (j = 0, x = -HalfWidth; j < nx; j++, x += hx)
                {
                        Camera(x, y, ray);
                        Color = Trace(Air, 1.0, ray);
                        Clip(Color);
                        c.Red = Color.x * 255;
                        c.Green = Color.y * 255;
                        c.Blue = Color.z * 255;
                        tga->PutPixel(c);
                        //                     DrawPixel ( j, i, Color );
                }
                if ((i % ProgressionBarStep) == 0)
                {
                        cout << "�";
                }
        }

        Ticks -= *TicksPtr;
        if (Ticks < 0l)
                Ticks = -Ticks;

        delete tga;

        //      getch ();
        //      SetMode ( 0x03 );
        cout << "\nEnd tracing.";
        //      DrawTargaFile ( PicFileName );
        cout << "\nRendering time : " << (int)(Ticks / 18) << " sec.";
}

void DistributedRenderScene(double HalfWidth, double HalfHeight, int nx, int ny, int nxSub, int nySub, char *PicFileName)
{
        cout << "\nBegin Tracing...\n\n";
        double x, y;                       // sample point
        Ray ray;                           // pixel ray
        double hx = 2.0 * HalfWidth / nx;  // pixel width
        double hy = 2.0 * HalfHeight / ny; // pixel height
        double hxSub = hx / nxSub;
        double hySub = hy / nySub;
        int i, j;
        int PrimarySamples = nxSub * nySub; // # of samples taken for each pixel
        Vector Color;
        Vector LineBuffer[800];
        long Ticks = *TicksPtr;
        TargaFile *tga = new TargaFile(PicFileName, nx, ny);
        RGB c;
        int ProgressionBarStep = (int)(ny / 80);
        if (ProgressionBarStep < 2)
        {
                ProgressionBarStep = 1;
        }

        //        SetMode ( 0x13 );
        //        SetPreviewPalette ();

        for (i = 0, y = HalfHeight; i < ny; i++, y -= hy)
        {
                for (j = 0, x = -HalfWidth; j < nx; j++, x += hx)
                {
                        double x1 = x - 0.5 * hx;
                        double y1 = y - 0.5 * hy;

                        Color = 0;

                        for (int iSub = 0; iSub < nxSub; iSub++)
                                for (int jSub = 0; jSub < nySub; jSub++)
                                {
                                        Camera(x1 + hxSub * (iSub + Rnd()), y1 + hySub * (jSub + Rnd()), ray);
                                        Color += Trace(Air, 1.0, ray);
                                }

                        Color /= PrimarySamples;
                        Clip(Color);
                        c.Red = Color.x * 255;
                        c.Green = Color.y * 255;
                        c.Blue = Color.z * 255;
                        tga->PutPixel(c);
                        //                       DrawPixel ( j, i, Color );
                }
                if ((i % ProgressionBarStep) == 0)
                {
                        cout << "�";
                }
        }

        Ticks -= *TicksPtr;
        if (Ticks < 0l)
                Ticks = -Ticks;

        delete tga;
        //        getch ();
        //        SetMode ( 0x03 );
        printf("\nEnd tracing.");
        //        DrawTargaFile ( PicFileName );
        printf("\nElapsed time : %d sec. ", (int)(Ticks / 18));
}

void AdaptiveDistributedRenderScene(double HalfWidth, double HalfHeight, int nx, int ny, int nxSub, int nySub, double Variance, char *PicFileName)
{
        double x, y;                      // sample point
        Ray ray;                          // pixel ray
        double hx = 2.0 * HalfWidth / nx; // pixel size
        double hy = 2.0 * HalfHeight / ny;
        double hxSub = hx / nxSub;
        double hySub = hy / nySub;
        double Disp; // dispersion squared
        int i, j;
        Vector Color;
        Vector Sum;
        Vector Mean;
        int Count;
        long Ticks = *TicksPtr;
        TargaFile *tga = new TargaFile(PicFileName, nx, ny);
        RGB c;

        SetMode(0x13);
        SetPreviewPalette();

        for (i = 0, y = HalfHeight; i < ny; i++, y -= hy)
        {
                for (j = 0, x = -HalfWidth; j < nx; j++, x += hx)
                {
                        double x1 = x - 0.5 * hx;
                        double y1 = y - 0.5 * hy;
                        double d;

                        Sum = 0;
                        Disp = 0;
                        Count = 0;

                        do
                        {
                                for (int iSub = 0; iSub < nxSub; iSub++)
                                        for (int jSub = 0; jSub < nySub; jSub++)
                                        {
                                                Camera(x1 + hxSub * (iSub + Rnd()), y1 + hySub * (jSub + Rnd()), ray);

                                                Color = Trace(Air, 1.0, ray);
                                                Sum += Color;
                                                Disp += Color & Color;
                                                Count++;
                                        }

                                Mean = Sum / Count;
                                d = (Disp / Count - (Mean & Mean)) * Count / (Count - 1);
                        } while (d >= Variance && Count < 99);

                        Clip(Mean);
                        c.Red = Mean.x * 255;
                        c.Green = Mean.y * 255;
                        c.Blue = Mean.z * 255;
                        tga->PutPixel(c);

                        DrawPixel(j, i, Mean);
                }
        }

        Ticks -= *TicksPtr;
        if (Ticks < 0l)
                Ticks = -Ticks;

        delete tga;

        getch();
        SetMode(0x03);
        printf("\nEnd tracing.");
        DrawTargaFile(PicFileName);
        printf("\nElapsed time : %d sec. ", (int)(Ticks / 18));
}
