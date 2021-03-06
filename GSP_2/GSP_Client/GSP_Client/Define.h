#pragma once

#define KEY_LEFT			'4'
#define KEY_RIGHT		'6'
#define KEY_UP				'8'
#define KEY_DOWN		'2'
#define KEY_NONE			'0'

#define WINX 800
#define WINY 800

#define MAX_BUFFER	 1024

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = NULL; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = NULL; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = NULL; }

#define	DECLARE_SINGLE(Type)	\
	private:\
		Type();\
		~Type();\
	private:\
		static Type*	m_pInst;\
	public:\
		static Type* GetInst()\
		{\
			if(!m_pInst)\
				m_pInst	= new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}

#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInst	= NULL;
#define	GET_SINGLE(Type)		Type::GetInst()
#define	DESTROY_SINGLE(Type)	Type::DestroyInst()

#define	KEYDOWN(key)	CInput::GetInst()->KeyDown(key)
#define	KEYPUSH(key)	CInput::GetInst()->KeyPush(key)
#define	KEYUP(key)		CInput::GetInst()->KeyUp(key)