#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
#include <fstream>

using namespace Gdiplus;

WCHAR currentDirectory[MAX_PATH];

BOOL LoadCustomImage();
VOID ConvertImage(LPWSTR fileName);

int main()
{
	GetCurrentDirectory(MAX_PATH, currentDirectory);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	if (LoadCustomImage()) std::cout << "Done!\n";
	else std::cout << "Something went wrong!\n";
	
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

BOOL LoadCustomImage()
{
	OPENFILENAME ofn;
	TCHAR FileName[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(FileName, MAX_PATH);

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = L"Images\0*.*\0*.jpg\0*.bmp\0*.png\0*.wmf\0*.gif";
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if (GetOpenFileName(&ofn))
	{
		ConvertImage(ofn.lpstrFile);
	}
	else return FALSE;

	return TRUE;
}

VOID ConvertImage(LPWSTR fileName)
{
	std::string symbols = "@#%xo;:,. ";
	std::string result = "";

	Bitmap* bitmapToConvert = new Bitmap(fileName);

	Color pixelColor = NULL;

	INT temp = 0;

	for (UINT i = 0; i < bitmapToConvert->GetWidth(); i += 3)
	{
		for (UINT j = 0; j < bitmapToConvert->GetHeight(); j++)
		{
			bitmapToConvert->GetPixel(j, i, &pixelColor);

			temp = pixelColor.GetR() + pixelColor.GetG() + pixelColor.GetB();

			result += symbols[(symbols.length() * temp) / (256 * 3)];
		}

		result += '\n';
	}

	SetCurrentDirectory(currentDirectory);

	std::ofstream wFile;

	wFile.open("convertedImage.txt");

	if (wFile.is_open())
	{
		wFile << result;
		wFile.close();
	}
	else
	{
		std::cout << "Can`t open file...\n";
	}

	delete bitmapToConvert;
}
