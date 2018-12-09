#pragma once
const static float PI = 3.14159;

const float STEM_HEIGHT0 = -2.f;
const float STEM_HEIGHT = 2.f;
const float STEM_RADIUS = 0.2f;
const int STEM_POINTS_NUMBER = 20;

const float STALK_HEIGHT0 = STEM_HEIGHT0 + STEM_HEIGHT;
const float STALK_HEIGHT = 0.1f;
const float STALK_RADIUS = 0.7f;
const int STALK_POINTS_NUMBER = 20;

const static int PETAL_NUMBER = 6;

const float PETAL_HEIGHT0 = STALK_HEIGHT0 + STALK_HEIGHT;
const float PETAL_HEIGHT = 0.5f;
const float PETAL_LOWEST_WIDTH = 2 * PI * STALK_RADIUS / PETAL_NUMBER * 0.7f;
const float PETAL_BIGGEST_WIDTH = 2 * PI * STALK_RADIUS / PETAL_NUMBER;

const XMFLOAT4 yellowColor = XMFLOAT4(1.0, 1.0, 0.0, 1.0);
const XMFLOAT4 purpleColor = XMFLOAT4(0.5, 0.0, 0.5, 1.0);
const XMFLOAT4 greenColor = XMFLOAT4(0.0, 1.0, 0.0, 1.0);
