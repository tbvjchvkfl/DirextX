#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename) : mpFramework(pFramework), mpBitmap(), mX(), mY(), mOpacity(1.0f)
{
	// ��Ʈ�� �Ŵ����� ��Ʈ���� �ҷ���.
	mpBitmap = BitmapManager::Instance().LoadBitmap(filename);
}

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity) : Actor(pFramework, filename)
{
	mX = x;
	mY = y;
	mOpacity = opacity;
}

Actor::~Actor()
{
}

void Actor::Draw(float x, float y, float opacity)
{
	// ���� Ÿ�� ȹ��
	auto pRT = mpFramework->GetRenderTarget();
	if (pRT == nullptr)
	{
		return;
	}

	// ��Ʈ�� ũ�� �� �׷��� ���� ����
	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rect{ x,y,static_cast<float>(x + size.width), static_cast<float>(y + size.height) };

	// ��Ʈ���� ����Ÿ�ٿ� �׸���
	pRT->DrawBitmap(mpBitmap, rect, opacity);
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity);
}
