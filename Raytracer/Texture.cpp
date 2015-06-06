#include "Texture.h"


FileTexture::FileTexture(char* file) {
	if (!loadTexture(file)) {
		printf("Error loading texture.\n");
	} 
}

FileTexture::~FileTexture() {
	free(m_bmp->data);
	free(m_bmp);
}

bool FileTexture::loadTexture(char* file) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(file);
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(file);
	}
	if (fif == FIF_UNKNOWN) return false; // can't read file

	m_bmp = FreeImage_Load(fif, file); // load texture
	return true;
}



Vector3 FileTexture::getColor2D(TexPoint& t) const {
	float u = t.u();
	float v = t.v();

	int width = FreeImage_GetWidth(m_bmp);
	int height = FreeImage_GetHeight(m_bmp);

	float x = u * (float)width;
	float y = v * (float)height;

	// Bilinear interpolation
	int x_left = (int)x;
	int x_right = x + 1;
	x_left %= width;
	x_right %= width;

	float x_offset = x - (float)x_left;

	int y_bot = (int)y;
	int y_top = y + 1;
	y_bot %= height;
	y_top %= height;

	float y_offset = y - (float)y_bot;


	Vector3 res = (this->pixelColor(x_left, y_bot) * (1.0f - x_offset) + pixelColor(x_right, y_bot) * x_offset) * (1.0f - y_offset)
		+ (pixelColor(x_left, y_top) * (1.0f - x_offset) + pixelColor(x_right, y_top) * x_offset) * y_offset;

	return res;
}


Vector3 FileTexture::pixelColor(int x, int y) const {
	Vector3 color(0.0f);

	if (FreeImage_GetImageType(m_bmp) == FIT_BITMAP) {
		RGBQUAD value;
		FreeImage_GetPixelColor(m_bmp, x, y, &value);
		color.x = (float)value.rgbRed / 255.0f;
		color.y = (float)value.rgbGreen / 255.0f;
		color.z = (float)value.rgbBlue / 255.0f;
	} else if (FreeImage_GetImageType(m_bmp) == FIT_RGBF) {
		FIRGBF value = ((FIRGBF*)FreeImage_GetScanLine(m_bmp, y))[x];
		color.x = value.red;
		color.y = value.green;
		color.z = value.blue;
	}
	return color;
}