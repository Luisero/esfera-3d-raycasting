#include <iostream>
#include <fstream>

int main()
{
    std::ofstream image("image.ppm");
    const int width = 2, height = 2;

    if(image.is_open())
    {
        image << "P3\n";
        image << width << " " << height << "\n";
        image << 255 << "\n";
        image << "125 0 0 255 0 0 \n";
        image << "100 255 0 255 0 0 \n";




        image.close();
    }
}
