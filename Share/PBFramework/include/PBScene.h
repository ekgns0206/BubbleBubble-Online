#ifndef __PBSCENE_H__
#define	__PBSCENE_H__

#include "PBObject.h"
#include "PBDeviceContext.h"
#include <Windows.h>
#include <string>

namespace PB { namespace Windows {

class __declspec(dllexport) scene : public virtual object
{
public:
	scene();
	virtual ~scene();

public:
	virtual void			onCreate()							{};		// �ʱ�ȭ �̺�Ʈ �ڵ鷯
	virtual void			onFrameMove(float elapsedTime)		{};		// ������ �ϱ� ������ �̺�Ʈ �ڵ鷯
	virtual void			onFrameRender(device_context& dc, float elapsedTime)	{};		// ������ �̺�Ʈ �ڵ鷯
	virtual void			onDestroy()							{};		// ������ �̺�Ʈ �ڵ鷯
	virtual void			onKeyboard(char vk, bool isDown)	{};		// Ű���� �̺�Ʈ �ڵ鷯
};

} }

#endif // !__PBSCENE_H__