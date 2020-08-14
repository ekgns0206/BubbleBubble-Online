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
	virtual void			onCreate()							{};		// 초기화 이벤트 핸들러
	virtual void			onFrameMove(float elapsedTime)		{};		// 렌더링 하기 이전의 이벤트 핸들러
	virtual void			onFrameRender(device_context& dc, float elapsedTime)	{};		// 렌더링 이벤트 핸들러
	virtual void			onDestroy()							{};		// 릴리즈 이벤트 핸들러
	virtual void			onKeyboard(char vk, bool isDown)	{};		// 키보드 이벤트 핸들러
};

} }

#endif // !__PBSCENE_H__