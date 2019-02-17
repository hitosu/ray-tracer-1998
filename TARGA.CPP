#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <sys\stat.h>

TargaFile ::TargaFile(char *name, int width, int height, char *comment)
{
        _chmod(name, 1, 0); // reset file's attributes
        unlink(name);       // remove file

        file = open(name, O_WRONLY | O_BINARY | O_CREAT, S_IWRITE);
        BufSize = 1000;
        Buffer = new RGB[BufSize];
        pos = 0;

        memset(&Hdr, '\0', sizeof(Hdr));

        Hdr.DataType = 2;
        Hdr.Width = width;
        Hdr.Height = height;
        Hdr.DataBits = 24;
        Hdr.ImType = 32;

        if (comment[0] != '\0')
                Hdr.TextSize = strlen(comment) + 1;

        write(file, &Hdr, sizeof(Hdr));

        if (Hdr.TextSize > 0)
                write(file, comment, Hdr.TextSize);
}

TargaFile ::~TargaFile()
{
        if (pos > 0)
                Flush();

        delete Buffer;
        close(file);
}

void TargaFile ::PutPixel(RGB color)
{
        Buffer[pos].Red = color.Blue; // swap red & blue colors
        Buffer[pos].Green = color.Green;
        Buffer[pos].Blue = color.Red;

        if (++pos >= BufSize) // flush buffer if full
                Flush();
}

void TargaFile ::Flush()
{
        write(file, Buffer, pos * sizeof(RGB));
        pos = 0;
}
