
ColorTable ::ColorTable(int size)
{
        Entries = new ColorTableEntry[MaxEntries = size];
        ColorEntries = 0;
}

void ColorTable ::AddEntry(double a, double b, Vector c1, Vector c2)
{
        if (ColorEntries < MaxEntries - 1)
        {
                Entries[ColorEntries].ta = a;
                Entries[ColorEntries].tb = b;
                Entries[ColorEntries].ca = c1;
                Entries[ColorEntries].cb = c2;
                ColorEntries++;
        }
}

Vector ColorTable ::FindColor(double value)
{
        if (ColorEntries < 1)
                return Vector(0);

        if (value <= Entries[0].ta)
                return Entries[0].ca;

        for (int i = 0; i < ColorEntries; i++)
                if (value <= Entries[i].tb)
                {
                        double t = (value - Entries[i].ta) / (Entries[i].tb - Entries[i].ta);

                        return (1 - t) * Entries[i].ca + t * Entries[i].cb;
                }

        return Entries[ColorEntries - 1].cb;
}
