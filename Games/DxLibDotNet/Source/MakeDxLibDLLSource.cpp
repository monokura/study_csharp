#include <stdio.h>
#include "TextAnalyse.h"
#include <windows.h>
#include <string.h>
#include <malloc.h>
#include <varargs.h>

// ������ǂݍ��ݗp�N���X�I�u�W�F�N�g
TextAnalyse dhtxt, dxtxt, ttxt;

// �֐������
struct FUNCTIONNAME
{
	int				Counter;				// �����֐��̐�
	char			Name[ 64 ];				// �֐���
} ;

// ���C���f�[�^
struct MAIN
{
	char			*Struct;				// DxLib.h �̍\���̂���`����Ă��镔���̕�����
	char			*Function;				// DxLib.h �̊֐�����`����Ă��镔���̕�����

	int				FuncNameNum ;			// �֐����̐�
	FUNCTIONNAME	FuncName[ 65536 ] ;		// �֐������
} dat;

// �֐��̈����Ɋւ�����
struct PARAMETER
{
	bool			ValidDefault;			// �f�t�H���g�������L�����ǂ���
	bool			Out;					// out �������ǂ���
	char			Default[64];			// �f�t�H���g�����̕�����
	char			CPPType[64];			// C++ �ł̌^
	char			CSType[64];				// C# �ł̌^
	char			Name[64];				// ������
};

// �֐��Ɋւ�����
struct FUNCTION
{
	bool			Void;					// �߂�l�̌^�� void �^���ǂ���
	char			CPPType[64];			// C++ �ł̖߂�l�̌^
	char			CSType[64];				// C# �ł̖߂�l�̌^
	char			Name[64];				// �֐���
	
	PARAMETER		Parameter[64];			// �����̏��
	int				ParameterNum;			// �����̐�
	int				DefaultNum;				// �f�t�H���g�����̐�
};

// �^�ϊ��Ŏg�p������
struct TYPEINFO
{
	const char		*CPPName ;				// C++ �ł̌^��
	const char		*CSName ;				// C# �ł̌^��
	int				Size ;					// �^�̃T�C�Y( byte�� )
} ;


// �^�̕ϊ����
TYPEINFO TypeInfoDim[] =
{
	"void",						NULL,					0,
	"HWND",						"System.IntPtr",		4,
	"HICON",					"System.IntPtr",		4,
	"DX_DIRECT3DDEVICE9",		"System.IntPtr",		4,
	"DX_DIRECT3DSURFACE9",		"System.IntPtr",		4,
	"DX_CHAR",					"byte",					1,
	"WORD",						"ushort",				2,
	"BYTE",						"byte",					1,
	"LONGLONG",					"long",					8,
	"ULONGLONG",				"ulong",				8,
	"size_t",					"uint",					4,
	"int",						NULL,					4,
	"short",					NULL,					2,
	"char",						NULL,					1,
	"TCHAR",					"char",					1,
	"float",					NULL,					4,
	"double",					NULL,					8,
	"VECTOR",					NULL,					12,
	"MATRIX",					NULL,					64,
	"RECT",						NULL,					16,
	"DATEDATA",					NULL,					24,
	"IPDATA",					NULL,					4,
	"IPDATA_IPv6",				NULL,					16,
	"COLOR_F",					NULL,					16,
	"FLOAT4",					NULL,					16,
	"COLOR_U8",					NULL,					4,
	"UV",						NULL,					8,
	"BOOL",						"int",					4,
	"COLORDATA",				NULL,					1064,
	"IMEINPUTCLAUSEDATA",		NULL,					8,
	"HITRESULT_LINE",			NULL,					16,
	"MV1_COLL_RESULT_POLY",		NULL,					72,
	"MV1_COLL_RESULT_POLY_DIM",	NULL,					8,
	"MV1_REF_VERTEX",			NULL,					40,
	"MV1_REF_POLYGON",			NULL,					48,
	"DISPLAYMODEDATA",			NULL,					16,
	NULL, NULL, -1,
} ;

// DLL �ɂ̂ݏo�͂��� C# �p�ɂ͏o�͂��Ȃ��֐��̃��X�g
char *DLLOnlyFunction[] =
{
	"ClearDrawScreen", "ClearDrawScreenZBuffer",
	"GetTexPixelFormat", "GetTexColorData", "LoadGraphToResource", "GetWindowSizeChangeEnableFlag",
	"DrawChipMap", "BltBaseImage", "CreateGraphFromGraphImage",
	"ReCreateGraphFromGraphImage", "CreateDivGraphFromGraphImage", "ReCreateDivGraphFromGraphImage",
	"MemStreamOpen", "MemStreamClose",
	"GetDrawTargetSurface", "GetPrimarySurface", "GetBackSurface",
	"GetWorkSurface", "GetUseDDrawObj", "GetPixelFormat",
	"GetOverlayPixelFormat", "GetDirectDrawCaps", "GetDrawScreenDC",
	"GetDirectDrawDeviceGUID", "GetUseD3DDevObj", "GetVertexBuffer",
	"GetTexPixelFormat", "GetTexColorData", "GetTexPixelFormat",
	"GetTexColorData", "GetTexPixelFormat", "GetTexColorData",
	"GetZBufferPixelFormat", "GraphColorMatchBltVer2", "GetFullColorImage",
	"GetResourceIDString", "CreateDIBGraphVer2", "SetHookWinProc",
	"FileRead_getInfo", "FileRead_findFirst", "FileRead_findNext", "FileRead_findClose",
	NULL,
};

// ��؏o�͂��Ȃ��֐��̃��X�g
char *SkipFunction[] =
{
	"GraphFilter", "GraphFilterBlt", "GraphFilterRectBlt",
	"GraphBlend", "GraphBlendBlt", "GraphBlendRectBlt",
	"SetBlendGraphParam",
	"SetBeepFrequency", "PlayBeep", "StopBeep",
	"ErrorLogFmtAdd", "AppLogAdd", "printfDx", "FileRead_scanf", "DrawFormatString",
	"DrawFormatVString", "DrawFormatStringToHandle", "DrawFormatVStringToHandle",
	"DrawExtendFormatString", "DrawExtendFormatVString", "DrawExtendFormatStringToHandle",
	"DrawExtendFormatVStringToHandle", "DrawFormatStringMask", "DrawFormatStringMaskToHandle",
	"GetDrawFormatStringWidth", "GetDrawFormatStringWidthToHandle", 
	"GetDrawExtendFormatStringWidth", "GetDrawExtendFormatStringWidthToHandle",
	"DrawFormatStringToZBuffer", "DrawFormatVStringToZBuffer", "DrawFormatStringToHandleToZBuffer",
	"DrawFormatVStringToHandleToZBuffer", "DrawExtendFormatStringToZBuffer", "DrawExtendFormatVStringToZBuffer",
	"DrawExtendFormatStringToHandleToZBuffer", "DrawExtendFormatVStringToHandleToZBuffer",
	"SetKeyInputStringColor",
	NULL,
} ;

// ��������}�N���̃��X�g
char *SkipDefine[] =
{
	"STTELL(", "STSEEK(", "STREAD(", "STWRITE(", "STEOF(", "STCLOSE(",
	"STREAM_SEEKTYPE_SET", "STREAM_SEEKTYPE_END", "STREAM_SEEKTYPE_CUR",
	"DEFAULT_FOV", "DEFAULT_TAN_FOV_HALF", "DEFAULT_NEAR", "DEFAULT_FAR",
	"DEFAULT_FONT_SIZE", "DEFAULT_FONT_THINCK", "DEFAULT_FONT_TYPE", "DEFAULT_FONT_EDGESIZE",
	NULL,
};


int		AddFunctionName( const char *Name ) ;								// �֐�����ǉ�����( �߂�l�F�������O�̊֐��̐� )

int		CheckSkipFunc( char *str );											// �w��̊֐����X�L�b�v���邩�ǂ����𒲂ׂ�( 0:�X�L�b�v���Ȃ�  1:DLL�ɂ̂ݏo�͂���  2:�X�L�b�v���� )
bool	CheckSkipDefine( char *str );										// �w��̃}�N���𖳎����邩�ǂ����𒲂ׂ�( true:�X�L�b�v����  false:�X�L�b�v���Ȃ� )
bool	CheckSkipType( TextAnalyse *txt, bool function, bool *unsafe );		// �^���炻�̊֐���C#�̃v���O�����ɏo�͂��ׂ����𒲂ׂ�( true:�o�͂��Ȃ�  false:�o�͂��� )
bool	CheckSkipStruct( char *source );									// �w��̍\���̂̒��g���疳������\���̂��ǂ����𔻒f����( true:�X�L�b�v����  false:�X�L�b�v���Ȃ� )

bool	GetDirectXDefineStr( char *search, char *buffer );					// �w���DirectX�}�N���̐��l��������擾����( true:����  false:���s )
void	GetNumberOrIdentifier( char *src, char *dest );						// �w��̕����񂩂琔�����͎��ʎq�𔲂��o��

int		TypeConv( TextAnalyse *txt, char *CSType, bool UseSize, char *CPPType, bool *pOutFlag, bool Struct, bool Unsafe );		// C++ �p�̌^������� C# �p�̌^������ɕϊ�����( �߂�l�F�^�̃T�C�Y( byte�� ) )
void	Conv16To10( char *src, char *dest );								// �P�U�i���̕�������P�O�i���̕�����ɕϊ�����
int		ConvTokenStr( int bufferPos, char *buffer, char *src );				// �w��̕����񒆂̍\���ԂɃX�y�[�X�����镶����ɕϊ�����
void	CreateTokenStr( void );												// �\���ԂɃX�y�[�X�����镶������쐬����
void	fprintf2( FILE *fp1, FILE *fp2, const char *FormatStr, ... );		// ��̃t�@�C���|�C���^�ɓ������e���v�����g�o�͂���

void	OutputDefineStr( FILE *cs ) ;										// �萔��`���o�͂���
void	OutputStructStr( FILE *cs ) ;										// �\���̒�`���o�͂���
void	OutputBetaFunctionStr( FILE *cs, FILE *sc, FILE *def, FILE *def64, FILE *hd ) ;	// �x�^�o�͂̊֐���`���o�͂���
void	OutputFunctionStr( FILE *cs, FILE *sc, FILE *def, FILE *def64, FILE *hd ) ;		// �֐���`���o�͂���
void	OutputCSFunctionDefine( FILE *cs, char *FuncName, char *CSRet, char *CSParamDefine, bool Unsafe = false );	// �蓮�֐���`�̏o�͂��y�ɂ���֐�
void	OutputCSFunctionCode( FILE *cs, char *FuncCode1, char *FuncCode2 );	// �蓮�֐��R�[�h�̏o�͂��y�ɂ���֐�
void	OutputCSFunctionCode2( FILE *cs, char *FuncCode1, char *FuncCode2 );	// �蓮�֐��R�[�h�̏o�͂��y�ɂ���֐�


// �֐�����ǉ�����( �߂�l�F�������O�̊֐��̐� )
int AddFunctionName( const char *Name )
{
	int i ;

	// ���ɓ����֐��������邩���ׂ�
	for( i = 0 ; i < dat.FuncNameNum ; i ++ )
	{
		if( strcmp( dat.FuncName[ i ].Name, Name ) == 0 )
			break ;
	}

	// ����������V�K�ɒǉ�
	if( i == dat.FuncNameNum )
	{
		dat.FuncName[ dat.FuncNameNum ].Counter = 1 ;
		strcpy( dat.FuncName[ dat.FuncNameNum ].Name, Name ) ;
		dat.FuncNameNum ++ ;
		return 1 ;
	}
	else
	{
		// �������瓯���֐��̐����C���N�������g
		dat.FuncName[ i ].Counter ++ ;
		return dat.FuncName[ i ].Counter ;
	}
}

// �w��̊֐����X�L�b�v���邩�ǂ����𒲂ׂ�( 0:�X�L�b�v���Ȃ�  1:DLL�ɂ̂ݏo�͂���  2:�X�L�b�v���� )
int CheckSkipFunc( char *str )
{
	int i;
	
	for( i = 0; SkipFunction[i]; i++ )
	{
		if( strcmp( SkipFunction[i], str ) == 0 )
			return 2;
	}

	for( i = 0; DLLOnlyFunction[i]; i ++ )
	{
		if( strcmp( DLLOnlyFunction[i], str ) == 0 )
			return 1;
	}

	return 0;
}

// �w��̃}�N���𖳎����邩�ǂ����𒲂ׂ�( true:�X�L�b�v����  false:�X�L�b�v���Ȃ� )
bool CheckSkipDefine( char *str )
{
	int i;
	char Str[256];

	strcpy( Str, str );
	
	for( i = 0; SkipDefine[i]; i ++ )
	{
		if( strcmp( SkipDefine[i], Str ) == 0 )
			return true;
	}

	return false;
}

// �^���炻�̊֐���C#�̃v���O�����ɏo�͂��ׂ����𒲂ׂ�( true:�o�͂��Ȃ�  false:�o�͂��� )
bool CheckSkipType( TextAnalyse *txt, bool function, bool *unsafe )
{
	char *base;
	char Str[256], Str2[256], Str3[256];

	base = txt->getpos();
	txt->getstr( Str );

	// const �̕�����̓X�L�b�v����
	if( strcmp( Str, "const" ) == 0 )
	{
		txt->getstr( Str );
	}

	txt->getstr( Str2 );
	txt->getstr( Str3 );
	txt->move( base );

	// �|�C���^�̃|�C���^�̓X�L�b�v�Ώ�
	if( strcmp( Str2, "*" ) == 0 && strcmp( Str3, "*" ) == 0 ) return true;

	// �ϒ��������X�L�b�v�Ώ�
	if( strcmp( Str, "..." ) == 0 ) return true;

	// �ȉ��̌^���X�L�b�v�Ώ�
	if( strcmp( Str, "HMODULE" ) == 0 ) return true;
//	if( strcmp( Str, "HWND" ) == 0 ) return true;
	if( strcmp( Str, "HDC" ) == 0 ) return true;
	if( strcmp( Str, "STREAMDATASHREDTYPE2" ) == 0 ) return true;
	if( strcmp( Str, "BASEIMAGE" ) == 0 ) return true;
	if( strcmp( Str, "GUID" ) == 0 ) return true;
//	if( strcmp( Str, "MATRIX" ) == 0 ) return true;
//	if( strcmp( Str, "COLORDATA" ) == 0 ) return true;
	if( strcmp( Str, "HBITMAP" ) == 0 ) return true;
	if( strcmp( Str, "BITMAPINFO" ) == 0 ) return true;
	if( strcmp( Str, "WAVEFORMATEX" ) == 0 ) return true;
	if( strcmp( Str, "STREAMDATA" ) == 0 ) return true;
	if( strncmp( Str, "D_", 2 ) == 0 ) return true;

	// �֐��̖߂�l�̌^�`�F�b�N���ǂ����ŏ����𕪊�
	if( function )
	{
		// �|�C���^��Ԃ��֐��͌^����ł� unsafe �̂ݗL��
		if( strcmp( Str2, "*" ) == 0 &&
			strcmp( Str, "DX_DIRECT3DDEVICE9" ) != 0 &&
			strcmp( Str, "DX_DIRECT3DSURFACE9" ) != 0 )
		{
			*unsafe = true ;
		}

		// �֐��̖߂�l���ȉ��̌^�ȊO�̏ꍇ�̓X�L�b�v
		if( strcmp( Str, "DWORD_PTR" ) != 0 &&
			strcmp( Str, "DWORD" ) != 0 &&
			strcmp( Str, "int" ) != 0 &&
			strcmp( Str, "void" ) != 0 &&
			strcmp( Str, "float" ) != 0 &&
			strcmp( Str, "FLOAT4" ) != 0 &&
			strcmp( Str, "BOOL" ) != 0 &&
			strcmp( Str, "LONGLONG" ) != 0 &&
			strcmp( Str, "ULONGLONG" ) != 0 &&
			strcmp( Str, "VECTOR" ) != 0 &&
			strcmp( Str, "HITRESULT_LINE" ) != 0 &&
			strcmp( Str, "MATRIX" ) != 0 &&
			strcmp( Str, "size_t" ) != 0 &&
			strcmp( Str, "COLOR_F" ) != 0 &&
			strcmp( Str, "COLOR_U8" ) != 0 &&
			strcmp( Str, "MV1_COLL_RESULT_POLY_DIM" ) != 0 &&
			strcmp( Str, "MV1_COLL_RESULT_POLY" ) != 0 &&
			strcmp( Str, "DISPLAYMODEDATA" ) != 0 &&
			strcmp( Str, "DX_DIRECT3DDEVICE9" ) != 0 &&
			strcmp( Str, "DX_DIRECT3DSURFACE9" ) != 0 ) return true;
	}
	else
	{
		// IPDATA �̃|�C���^�� void �^�|�C���^�� unsafe �̂ݗL��
		if( ( strcmp( Str, "IPDATA_IPv6" ) == 0 && strcmp( Str2, "*" ) == 0 ) || 
			( strcmp( Str, "IPDATA" ) == 0 && strcmp( Str2, "*" ) == 0 ) || 
			( strcmp( Str, "void" ) == 0   && strcmp( Str2, "*" ) == 0 ) )
		{
			*unsafe = true ;
		}
	}

	return false;
}

// �w��̍\���̂̒��g���疳������\���̂��ǂ����𔻒f����( true:�X�L�b�v����  false:�X�L�b�v���Ȃ� )
bool CheckSkipStruct( char *source )
{
	// �\���̂̒��Ɉȉ��̕����񂪊܂܂�Ă�����o�͂��Ȃ�
	if( strstr( source, "union" ) ) return true;
	if( strstr( source, "struct" ) ) return true;
	if( strstr( source, "[" ) ) return true;
//	if( strstr( source, "*" ) ) return true;
//	if( strstr( source, "COLORDATA" ) ) return true;
	if( strstr( source, "STREAMDATASHRED" ) ) return true;

	return false;
}

// �w���DirectX�}�N���̐��l��������擾����( true:����  false:���s )
bool GetDirectXDefineStr( char *search, char *buffer )
{
	char str[256], str2[256], searchstr[256];

	// �ŏ��Ɍ������镶������Z�b�g
	strcpy( searchstr, search );

	for(;;)
	{
		// DxDirectX.h �� DX_NOUSE_DIRECTX_SDK_FILE ���L�q����Ă���Ƃ���܂ňړ�����
		dxtxt.reset();
		dxtxt.search( "DX_NOUSE_DIRECTX_SDK_FILE" );

		// �������������������
		if( dxtxt.search( searchstr ) )
		{
			// ��������擾����
			dxtxt.getstr( str );

			// enum �̏ꍇ�� = �̌�̕�������擾����
			if( strcmp( str, "=" ) == 0 )
			{
				dxtxt.getstr( str );
			}

			// enum �̏ꍇ�͖��[�� , ���������肷��̂ŁA���ʎq�␔�l�Ɏg�p�ł��镶�������̕�������擾����
			GetNumberOrIdentifier( str, str2 );

			// �擪�����������ł͂Ȃ������ꍇ�͕ʂ̃}�N���Ɣ��f����
			if( str2[0] < '0' || str2[0] > '9' )
			{
				// �������镶�����ύX���Ă���Ɍ������s��
				strcpy( searchstr, str2 );
				continue;
			}

			// 16�i���̉\�����l����10�i���ɕϊ����鏈�����s��
			Conv16To10( str2, str );

			// ���l�������ۑ�����
			strcpy( buffer, str );

			// ����
			return true;
		}
		else
		{
			// ����������G���[
			return false;
		}
	}

	// �����ɗ��邱�Ƃ͂Ȃ����ǈꉞ�G���[
	return false;
}

// �w��̕����񂩂琔�����͎��ʎq�𔲂��o��
void GetNumberOrIdentifier( char *src, char *dest )
{
	// �w��̕����񂩂环�ʎq�Ɏg�p���邱�Ƃ��ł��镶���݂̂𔲂��o��
	for(;;)
	{
		if( *src == '\0' ) break;
		if( ( *src >= 'a' && *src <= 'z' ) ||
			( *src >= 'A' && *src <= 'Z' ) ||
			( *src >= '0' && *src <= '9' ) ||
			*src == '_' )
		{
			*dest = *src;
			dest ++;
		}
		src ++ ;
	}
	*dest = '\0';
}

// C++ �p�̌^������� C# �p�̌^������ɕϊ�����( �߂�l�F�^�̃T�C�Y( byte�� ) )
int TypeConv( TextAnalyse *txt, char *CSType, bool UseSize, char *CPPType, bool *pOutFlag, bool Struct, bool Unsafe )
{
	char Str1[256], Str2[256], Str3[256], Str4[256];
	char cstype[64];
	int i, k, cppobj_usenum = 1, typebytes;
	char *start;
	bool is_const;

	// ������p�z�񏉊���
	memset( Str1, 0, sizeof( Str1 ) );
	memset( Str2, 0, sizeof( Str2 ) );
	memset( Str3, 0, sizeof( Str3 ) );
	memset( Str4, 0, sizeof( Str4 ) );
	start = txt->getpos();
	txt->getstr( Str1 );
	txt->getstr( Str2 );
	txt->getstr( Str3 );
	txt->getstr( Str4 );
	txt->move( start );

	// �ŏ��� out �^�C�v�ł͂Ȃ��A�ɂ��Ă���
	if( pOutFlag ) *pOutFlag = false;

	// C# �ł̌^���擾���鏈��
	for(;;)
	{
		// const ���t���ꍇ�͕ʏ���
		is_const = false;
		if( strcmp( Str1, "const" ) == 0 )
		{
			is_const = true;

			txt->getstr( Str1 );

			start = txt->getpos();
			txt->getstr( Str1 );
			txt->getstr( Str2 );
			txt->getstr( Str3 );
			txt->getstr( Str4 );
			txt->move( start );

			// const void * �̏ꍇ�͓���Ȃ̂ł����ŏ������Ă��܂�
			if( strcmp( Str1, "void" ) == 0 && strcmp( Str2, "*" ) == 0 )
			{
				txt->getstr( Str4 );
				txt->getstr( Str4 );
				sprintf( CPPType, "const %s %s", Str1, Str2 );
				strcpy( CSType, "void * " );
				return 4;
			}

			// const IMEINPUTCLAUSEDATA * �̏ꍇ�͓���Ȃ̂ł����ŏ������Ă��܂�
			if( strcmp( Str1, "IMEINPUTCLAUSEDATA" ) == 0 && strcmp( Str2, "*" ) == 0 && Struct )
			{
				txt->getstr( Str4 );
				txt->getstr( Str4 );
				sprintf( CPPType, "const %s %s", Str1, Str2 );
				strcpy( CSType, "uint " );
				return 4;
			}

			// const COLORDATA * �̏ꍇ�͓���Ȃ̂ł����ŏ������Ă��܂�
			if( strcmp( Str1, "COLORDATA" ) == 0 && strcmp( Str2, "*" ) == 0 && Struct )
			{
				txt->getstr( Str4 );
				txt->getstr( Str4 );
				sprintf( CPPType, "const %s %s", Str1, Str2 );
				strcpy( CSType, "uint " );
				return 4;
			}

			// const char ** �� const char * �̏ꍇ�͓���Ȃ̂ł����ŏ������Ă��܂�
			if( strcmp( Str1, "char" ) == 0 || strcmp( Str1, "TCHAR" ) == 0 )
			{
				if( strcmp( Str2, "*" ) == 0 && strcmp( Str3, "*" ) == 0 )
				{
					txt->getstr( Str4 );
					txt->getstr( Str4 );
					txt->getstr( Str4 );
					sprintf( CPPType, "const char **" );
					strcpy( CSType, "uint " );
					return 4;
				}
				if( strcmp( Str2, "*" ) == 0 )
				{
					txt->getstr( Str4 );
					txt->getstr( Str4 );
					sprintf( CPPType, "const char *" );

					// �\���̂̏ꍇ�� uint
					if( Struct )
					{
						strcpy( CSType, "uint " );
					}
					else
					{
						strcpy( CSType, "string " );
					}
					return 4;
				}
			}
		}

		// unsigned ���t���ꍇ�̏���
		if( strcmp( Str1, "unsigned" ) == 0 )
		{
			cppobj_usenum = 2;
			if( strcmp( Str2, "int" ) == 0 )
			{
				strcpy( cstype, "uint" );
				typebytes = 4;
				break;
			}

			if( strcmp( Str2, "short" ) == 0 )
			{
				strcpy( cstype, "ushort" );
				typebytes = 2;
				break;
			}

			if( strcmp( Str2, "char" ) == 0 || strcmp( Str2, "TCHAR" ) == 0 )
			{
				strcpy( cstype, "byte" );
				typebytes = 1;
				break;
			}
		}

		if( strcmp( Str1, "DWORD_PTR" ) == 0 )
		{
			strcpy( cstype, "uint" );
			typebytes = 4;
			break;
		}

		// DWORD �� GetColor �̊֐��̖߂�l���ǂ����ŏ����𕪊�
		if( strcmp( Str1, "DWORD" ) == 0 )
		{
			if( strcmp( Str2, "GetColor" ) == 0 )
			{
				strcpy( cstype, "int" );
			}
			else
			{
				strcpy( cstype, "uint" );
			}
			typebytes = 4;
			break;
		}

		// ���̑��̈�ʓI�ȕϊ��̓e�[�u�����g���čs��
		for( k = 0 ; TypeInfoDim[ k ].CPPName ; k ++ )
		{
			if( strcmp( TypeInfoDim[ k ].CPPName, Str1 ) == 0 ) break ;
		}
		if( TypeInfoDim[ k ].CPPName != NULL && k == 3 )
		{
			k = k ;
		}

		// �e�[�u���̒����猩�������ꍇ�͂��̏����Z�b�g
		if( TypeInfoDim[ k ].CPPName != NULL )
		{
			strcpy( cstype, TypeInfoDim[ k ].CSName == NULL ? TypeInfoDim[ k ].CPPName : TypeInfoDim[ k ].CSName ) ;
			typebytes = TypeInfoDim[ k ].Size ;
		}
		else
		{
			// ������Ȃ������ꍇ�̏���
			if( UseSize == true )
			{
				printf( "%s error\n", Str1 );
				for(;;){}
				break;
			}
			strcpy( cstype, Str1 );
			typebytes = 4;
		}
		break;
	}

	// C++ �̌^��������쐬
	strcpy( CPPType, "" );
	if( is_const )
	{
		strcat( CPPType, "const " );
	}
	for( i = 0; i < cppobj_usenum; i ++ )
	{
		txt->getstr( Str1 );
		strcat( CPPType, Str1 );
		strcat( CPPType, " " );
	}

	// �����|�C���^�̏ꍇ�͏����𕪊�
	if( txt->nextchar() == '*' )
	{
		txt->getstr( Str1 );

		// �\���̂̃����o�ϐ��̏ꍇ�� unsigned int �^�̕ϐ��Ƃ���
		if( Struct )
		{
			strcpy( CSType, "uint " ) ;
		}
		else
		{
			// ������̏ꍇ�� System.Text.StringBuilder �^�Ƃ���
			if( strcmp( cstype, "char" ) == 0 )
			{
				sprintf( CSType, "System.Text.StringBuilder " );
			}
			else
			// void �^�̏ꍇ�� void �^�|�C���^��
			if( strcmp( cstype, "void" ) == 0 )
			{
				sprintf( CSType, "void * " );
			}
			else
			// �o�͂� System.IntPtr �̏ꍇ�͂��̂܂܏o��
			if( strcmp( cstype, "System.IntPtr" ) == 0 )
			{
				strcpy( CSType, cstype ) ;
			}
			else
			// ����ȊO�̏ꍇ�� unsafe �̏ꍇ�͂��̂܂܃|�C���^�Ƃ���
			if( Unsafe )
			{
				sprintf( CSType, "%s * ", cstype );
			}
			else
			// unsafe �ł͂Ȃ��ꍇ�� out �t���̌^�Ƃ���
			{
				sprintf( CSType, "out %s ", cstype );
			}
			if( pOutFlag ) *pOutFlag = true;
		}

		// �|�C���^�̏ꍇ�̓T�C�Y�� 4byte
		typebytes = 4;

		// C++ �̌^�ɂ͕��ʂ� * ��ǉ�����
		strcat( CPPType, "* " );
	}
	else
	{
		// �|�C���^�ł͂Ȃ��ꍇ�͓���ꂽ�^�����̂܂ܕԂ�
		sprintf( CSType, "%s ", cstype );
	}

	// �^�̃T�C�Y��Ԃ�
	return typebytes;
}

// �P�U�i���̕�������P�O�i���̕�����ɕϊ�����
void Conv16To10( char *src, char *dest )
{
	// 16�i���̕�����ł͂Ȃ������炽���R�s�[���邾��
	if( src[0] == '0' && src[1] == 'x' )
	{
		int i, j, num[16], digit, total;

		// �e���̐��l�𓾂�
		src += 2;
		i = 0;
		for( i = 0; *src != '\0'; src++, i++)
		{
			if( *src >= 'a' && *src <= 'f' )
			{
				num[i] = *src - 'a' + 10;
			}
			else
			if( *src >= 'A' && *src <= 'F' )
			{
				num[i] = *src - 'A' + 10;
			}
			else
			{
				num[i] = *src - '0';
			}
		}
		digit = i;

		// ���l�𓾂�
		j = 0;
		total = 0;
		for( i = 0; i < digit; i ++, j += 4 )
		{
			total |= num[digit-1-i] << j;
		}

		// 10�i���̕�����𓾂�
		itoa( total, dest, 10 );
	}
	else
	{
		strcpy( dest, src );
	}
}

// �w��̕����񒆂̍\���ԂɃX�y�[�X�����镶����ɕϊ�����
int ConvTokenStr( int bufferPos, char *buffer, char *src )
{
	for(;;)
	{
		switch( src[0] )
		{
		case '\0':
			buffer[bufferPos] = ' ';
			bufferPos ++ ;
			return bufferPos;

		case '(': case ')': case '[': case ']':
		case '*': case ',': case ';':
			buffer[bufferPos]   = ' ';
			buffer[bufferPos+1] = src[0];
			buffer[bufferPos+2] = ' ';
			bufferPos += 3;
			break;

		default:
			buffer[bufferPos] = src[0];
			bufferPos ++ ;
			break;
		}
		src ++ ;
	}
}

// �\���ԂɃX�y�[�X�����镶������쐬����
void CreateTokenStr( void )
{
	char str1[256] ;
	int pos;

	dat.Struct   = (char *)malloc( 1024 * 1024 * 16 );
	dat.Function = (char *)malloc( 1024 * 1024 * 16 );

	// �\���̂���`����Ă���Ƃ���܂ňړ�����
	dhtxt.reset() ;
	dhtxt.search( "DX_STRUCT_START" );
	pos = 0;
	for(;;)
	{
		if( dhtxt.getstr( str1 ) == false ) break;
		if( strcmp( str1, "DX_STRUCT_END" ) == 0 ) break;
		pos = ConvTokenStr( pos, dat.Struct, str1 );
	}
	dat.Struct[pos] = '\0';

	dhtxt.search( "DX_FUNCTION_START" );
	pos = 0;
	for(;;)
	{
		if( dhtxt.getstr( str1 ) == false ) break;
		if( strcmp( str1, "DX_FUNCTION_END" ) == 0 ) break;
		pos = ConvTokenStr( pos, dat.Function, str1 );
	}
	dat.Function[pos] = '\0';
}

// ��̃t�@�C���|�C���^�ɓ������e���v�����g�o�͂���
void fprintf2( FILE *fp1, FILE *fp2, const char *FormatStr, ... )
{
	va_list VaList;
	TCHAR String[ 2048 ];

	va_start( VaList, FormatStr );
	vsprintf( String, FormatStr, VaList );
	va_end( VaList );

	fprintf( fp1, String );
	fprintf( fp2, String );
}

// �c�w���C�u�����̒萔��`���o�͂���
void OutputDefineStr( FILE *cs )
{
	char Str[256], Str2[256] ;

	// TRUE �� FALSE �̒�`���o��
	fprintf( cs, "\t\tpublic const int TRUE  = 1;\n" );
	fprintf( cs, "\t\tpublic const int FALSE = 0;\n" );

	// define �����܂Ŕ��
	dhtxt.reset();
	dhtxt.search( "DX_DEFINE_START" );
	for(;;)
	{
		// #define ��T��
		dhtxt.getstr( Str );
		if( strcmp( Str, "#define" ) == 0 )
		{
			// DX_DEFINE_END ��������I��
			dhtxt.getstr( Str );
			if( strcmp( Str, "DX_DEFINE_END" ) == 0 ) break;

			// ��������}�N���������ꍇ�� continue
			if( CheckSkipDefine( Str ) ) continue;

			// �萔���̏o��
			fprintf( cs, "\t\tpublic const int %s = ", Str );

			// �萔�����̎擾
			dhtxt.getstr( Str );

			// ( �������ꍇ�� ) �����镔���܂ŏo��
			if( strcmp( Str, "(" ) == 0 )
			{
				fprintf( cs, "( " );
				for(;;)
				{
					dhtxt.getstr( Str );
					if( strcmp( Str, ")" ) == 0 ) break;
					fprintf( cs, "%s ", Str );
				}
				fprintf( cs, " );\n" );
			}
			else
			// ( �����l�������ꍇ�͐��l�����݂̂𔲂��o���A�P�U�i���̐��l���P�O�i���ɕϊ����ďo��
			if( Str[0] == '(' || ( Str[0] >= '0' && Str[0] <= '9' ) )
			{
				GetNumberOrIdentifier( Str, Str2 );
				Conv16To10( Str2, Str );
				fprintf( cs, "%s;\n", Str );
			}
			else
			// ����ȊO�̏ꍇ�� DirectX �̒�`�����̂܂܎g�p���Ă���̂� DxDirectX.h ����萔���擾���ďo��
			{
				if( GetDirectXDefineStr( Str, Str2 ) == false )
				{
					printf( "%s error\n", Str );
					for(;;){}
				}
				fprintf( cs, "%s;\n", Str2 );
			}
		}
	}
	fprintf( cs, "\n" );
}

// �\���̕������o�͂���
void OutputStructStr( FILE *cs )
{
	char Str[256], Str2[2024 * 10], Str3[256] ;
	char *start, *end ;
	int count, size ;

	// ���������ł͏o�͂ł��Ȃ��\���̂��x�^�o��
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct IPDATA_IPv6\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public byte Byte00;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(1)] public byte Byte01;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(2)] public byte Byte02;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(3)] public byte Byte03;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(4)] public byte Byte04;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(5)] public byte Byte05;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(6)] public byte Byte06;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(7)] public byte Byte07;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(8)] public byte Byte08;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(9)] public byte Byte09;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(10)] public byte Byte10;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(11)] public byte Byte11;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(12)] public byte Byte12;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(13)] public byte Byte13;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(14)] public byte Byte14;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(15)] public byte Byte15;\n" );
	fprintf( cs, "\t\t};\n\n" );
/*	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct IPDATA\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public byte d1;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(1)] public byte d2;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(2)] public byte d3;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(3)] public byte d4;\n" );
	fprintf( cs, "\t\t};\n\n" );*/
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct RECT\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public int left;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(4)] public int top;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(8)] public int right;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(12)] public int bottom;\n" );
	fprintf( cs, "\t\t};\n\n" );
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct MATRIX\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public float m00;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(4)] public float m01;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(8)] public float m02;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(12)] public float m03;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(16)] public float m10;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(20)] public float m11;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(24)] public float m12;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(28)] public float m13;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(32)] public float m20;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(36)] public float m21;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(40)] public float m22;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(44)] public float m23;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(48)] public float m30;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(52)] public float m31;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(56)] public float m32;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(60)] public float m33;\n" );
	fprintf( cs, "\t\t};\n\n" );
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct COLORDATA\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public short ColorBitDepth;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(2)] public short PixelByte;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(4)] public byte RedWidth ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(5)] public byte GreenWidth ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(6)] public byte BlueWidth ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(7)] public byte AlphaWidth ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(8)] public byte RedLoc ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(9)] public byte GreenLoc ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(10)] public byte BlueLoc ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(11)] public byte AlphaLoc ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(12)] public uint RedMask ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(16)] public uint GreenMask ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(20)] public uint BlueMask ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(24)] public uint AlphaMask ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(28)] public uint NoneMask ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(32)] public COLORPALETTEDATA Palette ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(1056)] public byte NoneLoc ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(1057)] public byte NoneWidth ;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(1060)] public int Format ;\n" );
	fprintf( cs, "\t\t};\n\n" );
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct MV1_COLL_RESULT_POLY\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public int HitFlag;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(4)] public VECTOR HitPosition;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(16)] public int FrameIndex;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(20)] public int PolyIndex;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(24)] public int MaterialIndex;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(28)] public VECTOR Position0;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(40)] public VECTOR Position1;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(52)] public VECTOR Position2;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(64)] public VECTOR Normal;\n" );
	fprintf( cs, "\t\t};\n\n" );
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct MV1_REF_POLYGON\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public ushort FrameIndex;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(2)] public ushort MaterialIndex;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(4)] public int VIndexTarget;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(8)] public int VIndex0;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(12)] public int VIndex1;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(16)] public int VIndex2;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(20)] public VECTOR MinPosition;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(32)] public VECTOR MaxPosition;\n" );
	fprintf( cs, "\t\t};\n\n" );
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct MV1_REF_VERTEX\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public VECTOR Position;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(12)] public VECTOR Normal;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(24)] public UV TexCoord0;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(32)] public UV TexCoord1;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(40)] public COLOR_U8 DiffuseColor;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(44)] public COLOR_U8 SpecularColor;\n" );
	fprintf( cs, "\t\t};\n\n" );
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct DINPUT_JOYSTATE\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public int X;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(4)] public int Y;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(8)] public int Z;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(12)] public int Rx;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(16)] public int Ry;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(20)] public int Rz;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(24)] public int Slider0;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(28)] public int Slider1;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(32)] public uint POV0;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(36)] public uint POV1;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(40)] public uint POV2;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(44)] public uint POV3;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(48)] public byte Buttons0;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(49)] public byte Buttons1;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(50)] public byte Buttons2;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(51)] public byte Buttons3;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(52)] public byte Buttons4;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(53)] public byte Buttons5;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(54)] public byte Buttons6;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(55)] public byte Buttons7;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(56)] public byte Buttons8;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(57)] public byte Buttons9;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(58)] public byte Buttons10;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(59)] public byte Buttons11;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(60)] public byte Buttons12;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(61)] public byte Buttons13;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(62)] public byte Buttons14;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(63)] public byte Buttons15;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(64)] public byte Buttons16;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(65)] public byte Buttons17;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(66)] public byte Buttons18;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(67)] public byte Buttons19;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(68)] public byte Buttons20;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(69)] public byte Buttons21;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(70)] public byte Buttons22;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(71)] public byte Buttons23;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(72)] public byte Buttons24;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(73)] public byte Buttons25;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(74)] public byte Buttons26;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(75)] public byte Buttons27;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(76)] public byte Buttons28;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(77)] public byte Buttons29;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(78)] public byte Buttons30;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(79)] public byte Buttons31;\n" );
	fprintf( cs, "\t\t};\n\n" );
	fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf( cs, "\t\tpublic struct XINPUT_STATE\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\t[FieldOffset(0)] public byte Buttons0;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(1)] public byte Buttons1;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(2)] public byte Buttons2;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(3)] public byte Buttons3;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(4)] public byte Buttons4;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(5)] public byte Buttons5;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(6)] public byte Buttons6;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(7)] public byte Buttons7;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(8)] public byte Buttons8;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(9)] public byte Buttons9;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(10)] public byte Buttons10;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(11)] public byte Buttons11;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(12)] public byte Buttons12;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(13)] public byte Buttons13;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(14)] public byte Buttons14;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(15)] public byte Buttons15;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(16)] public byte LeftTrigger;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(17)] public byte RightTrigger;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(18)] public short ThumbLX;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(20)] public short ThumbLY;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(22)] public short ThumbRX;\n" );
	fprintf( cs, "\t\t\t[FieldOffset(24)] public short ThumbRY;\n" );
	fprintf( cs, "\t\t};\n\n" );

	// ��͏���
	dhtxt.reset();
	ttxt.setmem( dat.Struct );
	for(;;)
	{
		// struct ��T��
		if( ttxt.getstr( Str ) == false ) break;
		if( strcmp( Str, "struct" ) == 0 )
		{
			// �\���̂̒��g�̕�������ۂ��� Str2 �ɃR�s�[����
			start = ttxt.getpos();
			ttxt.skipgroup( "{", "}" );
			end = ttxt.getpos();
			strncpy( Str2, start, end - start );
			Str2[end-start] = '\0';

			// �\���̂̒��g���疳������\���̂��ǂ������`�F�b�N����
			if( CheckSkipStruct( Str2 ) ) continue;

			// �\���̒�`�̏o��
			ttxt.getstr( Str );
			fprintf( cs, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
			fprintf( cs, "\t\tpublic struct %s\n", Str );
			fprintf( cs, "\t\t{\n" );
			ttxt.move( start );
			ttxt.search( "{" );

			count = 0;
			for(;;)
			{

				// } �ɗ�����I��
				start = ttxt.getpos();
				ttxt.getstr( Str );
				if( strcmp( Str, "}" ) == 0 ) break;
				ttxt.move( start );

				// �^�� C# �p�ɕϊ�����
				size = TypeConv( &ttxt, Str2, true, Str3, NULL, true, false );
				for(;;)
				{
					// �ϐ������擾
					ttxt.getstr( Str );

					// �ϐ��̒�`���o��
					fprintf( cs, "\t\t\t[FieldOffset(%d)] public %s %s;\n", count, Str2, Str );
					count += size;

					// ; �ɗ����烋�[�v�𔲂���
					ttxt.getstr( Str );
					if( strcmp( Str, ";" ) == 0 ) break;
				}
			}
			fprintf( cs, "\t\t};\n\n" );
		}
	}
	fprintf( cs, "\n" );
}

// �x�^�o�͂̊֐���`���o�͂���
void OutputBetaFunctionStr( FILE *cs, FILE *sc, FILE *def, FILE *def64, FILE *hd )
{
	fprintf( cs, "\t\tpublic static VECTOR VGet( float x, float y, float z )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf( cs, "\t\t\tResult.x = x ;\n" ) ;
	fprintf( cs, "\t\t\tResult.y = y ;\n" ) ;
	fprintf( cs, "\t\t\tResult.z = z ;\n" ) ;
	fprintf( cs, "\t\t\treturn Result ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;
	fprintf( cs, "\t\tpublic static VECTOR VAdd( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf( cs, "\t\t\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf( cs, "\t\t\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf( cs, "\t\t\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf( cs, "\t\t\treturn Result ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;
	fprintf( cs, "\t\tpublic static VECTOR VSub( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf( cs, "\t\t\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf( cs, "\t\t\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf( cs, "\t\t\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf( cs, "\t\t\treturn Result ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;
	fprintf( cs, "\t\tpublic static float VDot( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\treturn In1.x * In2.x + In1.y * In2.y + In1.z * In2.z ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;
	fprintf( cs, "\t\tpublic static VECTOR VCross( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf( cs, "\t\t\tResult.x = In1.y * In2.z - In1.z * In2.y ;\n" ) ;
	fprintf( cs, "\t\t\tResult.y = In1.z * In2.x - In1.x * In2.z ;\n" ) ;
	fprintf( cs, "\t\t\tResult.z = In1.x * In2.y - In1.y * In2.x ;\n" ) ;
	fprintf( cs, "\t\t\treturn Result ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;
	fprintf( cs, "\t\tpublic static VECTOR VScale( VECTOR In, float Scale )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf( cs, "\t\t\tResult.x = In.x * Scale ;\n" ) ;
	fprintf( cs, "\t\t\tResult.y = In.y * Scale ;\n" ) ;
	fprintf( cs, "\t\t\tResult.z = In.z * Scale ;\n" ) ;
	fprintf( cs, "\t\t\treturn Result ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;
	fprintf( cs, "\t\tpublic static float VSquareSize( VECTOR In )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\treturn In.x * In.x + In.y * In.y + In.z * In.z ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;
	fprintf( cs, "\t\tpublic static VECTOR VTransform( VECTOR InV, MATRIX InM )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf( cs, "\t\t\tResult.x = InV.x * InM.m00 + InV.y * InM.m10 + InV.z * InM.m20 + InM.m30 ;\n" ) ;
	fprintf( cs, "\t\t\tResult.y = InV.x * InM.m01 + InV.y * InM.m11 + InV.z * InM.m21 + InM.m31 ;\n" ) ;
	fprintf( cs, "\t\t\tResult.z = InV.x * InM.m02 + InV.y * InM.m12 + InV.z * InM.m22 + InM.m32 ;\n" ) ;
	fprintf( cs, "\t\t\treturn Result ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;
	fprintf( cs, "\t\tpublic static VECTOR VTransformSR( VECTOR InV, MATRIX InM )\n" ) ;
	fprintf( cs, "\t\t{\n" ) ;
	fprintf( cs, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf( cs, "\t\t\tResult.x = InV.x * InM.m00 + InV.y * InM.m10 + InV.z * InM.m20 ;\n" ) ;
	fprintf( cs, "\t\t\tResult.y = InV.x * InM.m01 + InV.y * InM.m11 + InV.z * InM.m21 ;\n" ) ;
	fprintf( cs, "\t\t\tResult.z = InV.x * InM.m02 + InV.y * InM.m12 + InV.z * InM.m22 ;\n" ) ;
	fprintf( cs, "\t\t\treturn Result ;\n" ) ;
	fprintf( cs, "\t\t}\n\n" ) ;

	fprintf( cs, "\n\n" );

	fprintf( cs, "#if DX_USE_UNSAFE\n" );
	fprintf( cs, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf( cs, "\t\tpublic unsafe delegate int SetActiveStateChangeCallBackFunctionCallback(int ActiveState, void* UserData);\n" );
	OutputCSFunctionDefine( cs, "dx_SetActiveStateChangeCallBackFunction", "int", "(SetActiveStateChangeCallBackFunctionCallback Callback, void* UserData)", true );
	fprintf( cs, "\t\tpublic unsafe static int SetActiveStateChangeCallBackFunction(SetActiveStateChangeCallBackFunctionCallback Callback, void* UserData)\n" );
	OutputCSFunctionCode( cs, "return dx_SetActiveStateChangeCallBackFunction", "(Callback, UserData)" );
	fprintf( cs, "\n" ) ;
	fprintf( cs, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf( cs, "\t\tpublic unsafe delegate void SetUseASyncChangeWindowModeFunctionCallback( void *Data );\n" );
	OutputCSFunctionDefine( cs, "dx_SetUseASyncChangeWindowModeFunction", "int", "( int Flag , SetUseASyncChangeWindowModeFunctionCallback CallBackFunction, void *Data )", true );
	fprintf( cs, "\t\tpublic unsafe static int SetUseASyncChangeWindowModeFunction( int Flag , SetUseASyncChangeWindowModeFunctionCallback CallBackFunction, void *Data )\n" );
	OutputCSFunctionCode( cs, "return dx_SetUseASyncChangeWindowModeFunction", "( Flag , CallBackFunction, Data )" );
	fprintf( cs, "#endif\n" );
	fprintf( cs, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf( cs, "\t\tpublic delegate void SetMenuItemSelectCallBackFunctionCallback( string ItemName, int ItemID );\n" );
	OutputCSFunctionDefine( cs, "dx_SetMenuItemSelectCallBackFunction", "int", "( SetMenuItemSelectCallBackFunctionCallback CallBackFunction )" );
	fprintf( cs, "\t\tpublic static int SetMenuItemSelectCallBackFunction( SetMenuItemSelectCallBackFunctionCallback CallBackFunction )\n" );
	OutputCSFunctionCode( cs, "return dx_SetMenuItemSelectCallBackFunction", "( CallBackFunction )" );
	fprintf( cs, "\n" ) ;
	fprintf( cs, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf( cs, "\t\tpublic delegate void SetWindowMenuCallback( ushort ID );\n" );
	OutputCSFunctionDefine( cs, "dx_SetWindowMenu", "int", "( int MenuID, SetWindowMenuCallback MenuProc )" );
	fprintf( cs, "\t\tpublic static int SetWindowMenu( int MenuID, SetWindowMenuCallback MenuProc )\n" );
	OutputCSFunctionCode( cs, "return dx_SetWindowMenu", "( MenuID, MenuProc )" );
	fprintf( cs, "\n" ) ;
	fprintf( cs, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf( cs, "\t\tpublic delegate void SetRestoreShredPointCallback();\n" );
	OutputCSFunctionDefine( cs, "dx_SetRestoreShredPoint", "int", "( SetRestoreShredPointCallback ShredPoint )" );
	fprintf( cs, "\t\tpublic static int SetRestoreShredPoint( SetRestoreShredPointCallback ShredPoint )\n" );
	OutputCSFunctionCode( cs, "return dx_SetRestoreShredPoint", "( ShredPoint )" );
	fprintf( cs, "\n" ) ;
	fprintf( cs, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf( cs, "\t\tpublic delegate void SetRestoreGraphCallbackCallback();\n" );
	OutputCSFunctionDefine( cs, "dx_SetRestoreGraphCallback", "int", "( SetRestoreGraphCallbackCallback Callback )" );
	fprintf( cs, "\t\tpublic static int SetRestoreGraphCallback( SetRestoreGraphCallbackCallback Callback )\n" );
	OutputCSFunctionCode( cs, "return dx_SetRestoreGraphCallback", "( Callback )" );
	fprintf( cs, "\n" ) ;
	OutputCSFunctionDefine( cs, "dx_ClearDrawScreen", "int", "( out RECT  ClearRect)" );
	fprintf( cs, "\t\tpublic static int  ClearDrawScreen( out RECT  ClearRect)\n" );
	OutputCSFunctionCode( cs, "return dx_ClearDrawScreen", "( out ClearRect )" );
	fprintf( cs, "\t\tpublic static int  ClearDrawScreen()\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\tRECT temp;\n" );
	fprintf( cs, "\t\t\n" );
	fprintf( cs, "\t\t\ttemp.left = -1;\n" );
	fprintf( cs, "\t\t\ttemp.top = -1;\n" );
	fprintf( cs, "\t\t\ttemp.right = -1;\n" );
	fprintf( cs, "\t\t\ttemp.bottom = -1;\n" );
	OutputCSFunctionCode2( cs, "return dx_ClearDrawScreen", "( out temp )" );
	fprintf( cs, "\t\t}\n" );
	fprintf( cs, "\t\t\n" );
	OutputCSFunctionDefine( cs, "dx_ClearDrawScreenZBuffer", "int", "( out RECT  ClearRect)" );
	fprintf( cs, "\t\tpublic static int  ClearDrawScreenZBuffer( out RECT  ClearRect)\n" );
	OutputCSFunctionCode( cs, "return dx_ClearDrawScreenZBuffer", "( out ClearRect )" );
	fprintf( cs, "\t\tpublic static int ClearDrawScreenZBuffer()\n" );
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\tRECT temp;\n" );
	fprintf( cs, "\t\t\n" );
	fprintf( cs, "\t\t\ttemp.left = -1;\n" );
	fprintf( cs, "\t\t\ttemp.top = -1;\n" );
	fprintf( cs, "\t\t\ttemp.right = -1;\n" );
	fprintf( cs, "\t\t\ttemp.bottom = -1;\n" );
	OutputCSFunctionCode2( cs, "return dx_ClearDrawScreenZBuffer", "( out temp )" );
	fprintf( cs, "\t\t}\n" );
	fprintf( cs, "\t\t\n" );

	OutputCSFunctionDefine( cs, "dx_SetKeyInputStringColor", "int", "( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF, ulong  SelectStrBackColor, ulong  SelectStrColor, ulong  SelectStrEdgeColor)" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , 0 , 0 , 0 , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , 0 , 0 , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , 0 , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , IMESelectWinF , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF, ulong  SelectStrBackColor)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , IMESelectWinF , SelectStrBackColor , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF, ulong  SelectStrBackColor, ulong  SelectStrColor)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , IMESelectWinF , SelectStrBackColor , SelectStrColor , 0xffffffffffffffff )" );
	fprintf( cs, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF, ulong  SelectStrBackColor, ulong  SelectStrColor, ulong  SelectStrEdgeColor)\n" );
	OutputCSFunctionCode( cs, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , IMESelectWinF , SelectStrBackColor , SelectStrColor , SelectStrEdgeColor )" );
	fprintf( hd, "int  __stdcall dx_SetKeyInputStringColor( ULONGLONG  NmlStr, ULONGLONG  NmlCur, ULONGLONG  IMEStr, ULONGLONG  IMECur, ULONGLONG  IMELine, ULONGLONG  IMESelectStr, ULONGLONG  IMEModeStr, ULONGLONG  NmlStrE = 0, ULONGLONG  IMESelectStrE = 0, ULONGLONG  IMEModeStrE = 0, ULONGLONG  IMESelectWinE = 0xffffffffffffffff, ULONGLONG  IMESelectWinF = 0xffffffffffffffff, ULONGLONG  SelectStrBackColor = 0xffffffffffffffff, ULONGLONG  SelectStrColor = 0xffffffffffffffff, ULONGLONG  SelectStrEdgeColor = 0xffffffffffffffff);\n" );
	fprintf( sc, "int  __stdcall dx_SetKeyInputStringColor( ULONGLONG  NmlStr, ULONGLONG  NmlCur, ULONGLONG  IMEStr, ULONGLONG  IMECur, ULONGLONG  IMELine, ULONGLONG  IMESelectStr, ULONGLONG  IMEModeStr, ULONGLONG  NmlStrE, ULONGLONG  IMESelectStrE, ULONGLONG  IMEModeStrE, ULONGLONG  IMESelectWinE, ULONGLONG  IMESelectWinF, ULONGLONG  SelectStrBackColor, ULONGLONG  SelectStrColor, ULONGLONG  SelectStrEdgeColor)\n" );
	fprintf( sc, "{\n" );
	fprintf( sc, "	return SetKeyInputStringColor( NmlStr, NmlCur, IMEStr, IMECur, IMELine, IMESelectStr, IMEModeStr, NmlStrE, IMESelectStrE, IMEModeStrE, IMESelectWinE, IMESelectWinF, SelectStrBackColor, SelectStrColor, SelectStrEdgeColor);\n" );
	fprintf( sc, "}\n" );
	fprintf2( def, def64, "\tdx_SetKeyInputStringColor\n" ) ;

	fprintf( cs, "\n\n" );

	OutputCSFunctionDefine( cs, "dx_GraphFilterS", "int", "( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterS", "( GrHandle, FilterType, 0, 0, 0, 0, 0, 0 )" );
	fprintf( cs, "\n" );
	OutputCSFunctionDefine( cs, "dx_GraphFilterBltS", "int", "( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, 0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\n" );
	OutputCSFunctionDefine( cs, "dx_GraphFilterRectBltS", "int", "( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, 0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\n" );
	OutputCSFunctionDefine( cs, "dx_GraphBlendS", "int", "( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, 0, 0, 0, 0, 0, 0 )" );
	fprintf( cs, "\n" );
	OutputCSFunctionDefine( cs, "dx_GraphBlendBltS", "int", "( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, 0, 0, 0, 0, 0, 0 )" );
	fprintf( cs, "\n" );
	OutputCSFunctionDefine( cs, "dx_GraphBlendRectBltS", "int", "( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType)\n" );
	OutputCSFunctionCode( cs, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, 0, 0, 0, 0, 0, 0 )" );
	fprintf( hd, "int  __stdcall dx_GraphFilterS( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf( hd, "int  __stdcall dx_GraphFilterBltS( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf( hd, "int  __stdcall dx_GraphFilterRectBltS( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf( hd, "int  __stdcall dx_GraphBlendS( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf( hd, "int  __stdcall dx_GraphBlendBltS( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf( hd, "int  __stdcall dx_GraphBlendRectBltS( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf( sc, "int  __stdcall dx_GraphFilterS( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf( sc, "{\n" );
	fprintf( sc, "	return GraphFilter( GrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, Param5 );\n" );
	fprintf( sc, "}\n" );
	fprintf( sc, "int  __stdcall dx_GraphFilterBltS( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf( sc, "{\n" );
	fprintf( sc, "	return GraphFilterBlt( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, Param5 );\n" );
	fprintf( sc, "}\n" );
	fprintf( sc, "int  __stdcall dx_GraphFilterRectBltS( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf( sc, "{\n" );
	fprintf( sc, "	return GraphFilterRectBlt( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, Param3, Param4, Param5 ) ;\n" );
	fprintf( sc, "}\n" );
	fprintf( sc, "int  __stdcall dx_GraphBlendS( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf( sc, "{\n" );
	fprintf( sc, "	return GraphBlend( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5 ) ;\n" );
	fprintf( sc, "}\n" );
	fprintf( sc, "int  __stdcall dx_GraphBlendBltS( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf( sc, "{\n" );
	fprintf( sc, "	return GraphBlendBlt( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5 ) ;\n" );
	fprintf( sc, "}\n" );
	fprintf( sc, "int  __stdcall dx_GraphBlendRectBltS( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf( sc, "{\n" );
	fprintf( sc, "	return GraphBlendRectBlt( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5 ) ;\n" );
	fprintf( sc, "}\n" );
	fprintf2( def, def64, "\tdx_GraphFilterS\n" );
	fprintf2( def, def64, "\tdx_GraphFilterBltS\n" );
	fprintf2( def, def64, "\tdx_GraphFilterRectBltS\n" );
	fprintf2( def, def64, "\tdx_GraphBlendS\n" );
	fprintf2( def, def64, "\tdx_GraphBlendBltS\n" );
	fprintf2( def, def64, "\tdx_GraphBlendRectBltS\n" );
	fprintf( cs, "\n\n" );

	OutputCSFunctionDefine( cs, "dx_SetBlendGraphParamS", "int", "( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf( cs, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf( cs, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf( cs, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0)\n" );
	OutputCSFunctionCode( cs, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, 0, 0, 0, 0, 0)" );
	fprintf( cs, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType )\n" );
	OutputCSFunctionCode( cs, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, 0, 0, 0, 0, 0, 0 )" );
	fprintf( cs, "\n" );
	fprintf( hd, "int  __stdcall dx_SetBlendGraphParamS( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf( sc, "int  __stdcall dx_SetBlendGraphParamS( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf( sc, "{\n" );
	fprintf( sc, "	return SetBlendGraphParam( BlendGraph, BlendType, Param0, Param1, Param2, Param3, Param4, Param5 );\n" );
	fprintf( sc, "}\n" );
	fprintf2( def, def64, "\tdx_SetBlendGraphParamS\n" );
	fprintf( cs, "\n\n" );

	fprintf( sc, "VECTOR __stdcall dx_VGet( float x, float y, float z )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\tVECTOR Result ;\n" ) ;
	fprintf( sc, "\tResult.x = x ;\n" ) ;
	fprintf( sc, "\tResult.y = y ;\n" ) ;
	fprintf( sc, "\tResult.z = z ;\n" ) ;
	fprintf( sc, "\treturn Result ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;
	fprintf( sc, "VECTOR __stdcall dx_VAdd( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\tVECTOR Result ;\n" ) ;
	fprintf( sc, "\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf( sc, "\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf( sc, "\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf( sc, "\treturn Result ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;
	fprintf( sc, "VECTOR __stdcall dx_VSub( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\tVECTOR Result ;\n" ) ;
	fprintf( sc, "\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf( sc, "\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf( sc, "\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf( sc, "\treturn Result ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;
	fprintf( sc, "float __stdcall dx_VDot( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\treturn In1.x * In2.x + In1.y * In2.y + In1.z * In2.z ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;
	fprintf( sc, "VECTOR __stdcall dx_VCross( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\tVECTOR Result ;\n" ) ;
	fprintf( sc, "\tResult.x = In1.y * In2.z - In1.z * In2.y ;\n" ) ;
	fprintf( sc, "\tResult.y = In1.z * In2.x - In1.x * In2.z ;\n" ) ;
	fprintf( sc, "\tResult.z = In1.x * In2.y - In1.y * In2.x ;\n" ) ;
	fprintf( sc, "\treturn Result ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;
	fprintf( sc, "VECTOR __stdcall dx_VScale( VECTOR In, float Scale )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\tVECTOR Result ;\n" ) ;
	fprintf( sc, "\tResult.x = In.x * Scale ;\n" ) ;
	fprintf( sc, "\tResult.y = In.y * Scale ;\n" ) ;
	fprintf( sc, "\tResult.z = In.z * Scale ;\n" ) ;
	fprintf( sc, "\treturn Result ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;
	fprintf( sc, "float __stdcall dx_VSquareSize( VECTOR In )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\treturn In.x * In.x + In.y * In.y + In.z * In.z ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;
	fprintf( sc, "VECTOR __stdcall dx_VTransform( VECTOR InV, MATRIX InM )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\tVECTOR Result ;\n" ) ;
	fprintf( sc, "\tResult.x = InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] + InM.m[3][0] ;\n" ) ;
	fprintf( sc, "\tResult.y = InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] + InM.m[3][1] ;\n" ) ;
	fprintf( sc, "\tResult.z = InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] + InM.m[3][2] ;\n" ) ;
	fprintf( sc, "\treturn Result ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;
	fprintf( sc, "VECTOR __stdcall dx_VTransformSR( VECTOR InV, MATRIX InM )\n" ) ;
	fprintf( sc, "{\n" ) ;
	fprintf( sc, "\tVECTOR Result ;\n" ) ;
	fprintf( sc, "\tResult.x = InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] ;\n" ) ;
	fprintf( sc, "\tResult.y = InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] ;\n" ) ;
	fprintf( sc, "\tResult.z = InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] ;\n" ) ;
	fprintf( sc, "\treturn Result ;\n" ) ;
	fprintf( sc, "}\n\n" ) ;



	fprintf( hd, "VECTOR __stdcall dx_VGet( float x, float y, float z ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VGet\n" ) ;
	fprintf( hd, "VECTOR __stdcall dx_VAdd( VECTOR In1, VECTOR In2 ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VAdd\n" ) ;
	fprintf( hd, "VECTOR __stdcall dx_VSub( VECTOR In1, VECTOR In2 ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VSub\n" ) ;
	fprintf( hd, "float __stdcall dx_VDot( VECTOR In1, VECTOR In2 ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VDot\n" ) ;
	fprintf( hd, "VECTOR __stdcall dx_VCross( VECTOR In1, VECTOR In2 ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VCross\n" ) ;
	fprintf( hd, "VECTOR __stdcall dx_VScale( VECTOR In, float Scale ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VScale\n" ) ;
	fprintf( hd, "float __stdcall dx_VSquareSize( VECTOR In ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VSquareSize\n" ) ;
	fprintf( hd, "VECTOR __stdcall dx_VTransform( VECTOR InV, MATRIX InM ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VTransform\n" ) ;
	fprintf( hd, "VECTOR __stdcall dx_VTransformSR( VECTOR InV, MATRIX InM ) ;\n" ) ;
	fprintf2( def, def64, "\tdx_VTransformSR\n" ) ;
}

// �֐���`���o�͂���
void OutputFunctionStr( FILE *cs, FILE *sc, FILE *def, FILE *def64, FILE *hd )
{
	char Str[256], Str2[2024 * 10], Str3[256], DllFuncName[ 256 ] ;
	char *start;
	bool unsafe;
	int i, j, k, l, size, funccount;
	FUNCTION Func;

	// �x�^�����̏o��
	OutputBetaFunctionStr( cs, sc, def, def64, hd ) ;

	ttxt.setmem( dat.Function );
	for(;;)
	{
		PARAMETER *par;
		char *FuncStart ;

		// ������擾
		if( ttxt.getstr( Str ) == false ) break;

		// �C�����C���֐��̓X�L�b�v
		if( strcmp( Str, "__inline" ) == 0 )
		{
			ttxt.search( "{" ) ;
			ttxt.search( "}" ) ;
			continue ;
		}
		else
		// extern �ȊO�̓X�L�b�v
		if( strcmp( Str, "extern" ) != 0 )
		{
			continue ;
		}

		// �����֐��̃J�E���^��������
		funccount = 0x0ccccccc ;

		// ���݂̕�����ʒu��ۑ�
		FuncStart = ttxt.getpos() ;

		// unsafe ���ǂ����̃t���O��|���Ă���
		unsafe = false ;

		// C# �p�ɏo�͂ł��Ȃ��߂�l�̌^�̏ꍇ�� DLL �ɂ̂ݏo�͂���
		if( CheckSkipType( &ttxt, true, &unsafe ) == true ) goto CPPOUTONLY;

		// �߂�l�̌^���擾����
		size = TypeConv( &ttxt, Func.CSType, false, Func.CPPType, NULL, false, unsafe );

		// void �^���ǂ�����ۑ�����
		Func.Void = size == 0;

		// �֐������擾
		ttxt.getstr( Func.Name );
/*
		if( strcmp( Func.Name, "BltBmpToGraph" ) == 0 )
		{
			size = size;
		}
*/
		// �֐����̓o�^
		funccount = AddFunctionName( Func.Name ) ;

		// �����Ώۂ̊֐����ǂ����𒲂ׂ�
		i = CheckSkipFunc( Func.Name ) ;

		// ���S�����̏ꍇ�͉��������Ɏ���
		if( i == 2 ) continue ;

		// �߂�l�� 1 �̏ꍇ�� DLL �ɂ̂ݏo�͂���
		if( i == 1 ) goto CPPOUTONLY;

		// �������
		{
			// �����̊J�n�ʒu��ۑ�
			ttxt.getstr( Str );
			start = ttxt.getpos();

			// �ŏ��̂Q�����񂩂���������������ׂ�
			ttxt.getstr( Str );
			ttxt.getstr( Str2 );
			ttxt.move( start );
			if( strcmp( Str, ")" ) == 0 || ( strcmp( Str, "void" ) == 0 && strcmp( Str2, ")" ) == 0 ) )
			{
				// �����������ꍇ
				Func.DefaultNum = 0;
				Func.ParameterNum = 0;
				if( strcmp( Str, "void" ) == 0 )
					ttxt.getstr( Str );
			}
			else
			{
				// ����������ꍇ
				i = 0;
				par = Func.Parameter;
				Func.DefaultNum = 0;
				for(;;)
				{
					// �����̌^�� C# �ɏo�͂ł��邩�ǂ������`�F�b�N
					if( CheckSkipType( &ttxt, false, &unsafe ) == true )
					{
						i = -1;
						break;
					}

					// C#�p�̈����̌^�𓾂�
					TypeConv( &ttxt, par->CSType, false, par->CPPType, &par->Out, false, unsafe );

					// ���������擾
					ttxt.getstr( par->Name );

					// �^�̎��� ( ������ꍇ�͊֐��|�C���^�Ȃ̂ŁA�����ϊ��ł� C# �ɏo�͂ł��Ȃ�
					if( strcmp( par->Name, "(" ) == 0 )
					{
						i = -1;
						break;
					}

					// �f�t�H���g���������邩�ǂ����̃`�F�b�N
					par->ValidDefault = false;
					if( ttxt.nextchar() == '=' )
					{
						// �f�t�H���g�������擾����
						ttxt.getstr( Str );
						par->ValidDefault = true;
						ttxt.getstr( par->Default );
						Func.DefaultNum ++ ;
					}

					par ++ ;
					i ++ ;
					ttxt.getstr( Str );
					if( strcmp( Str, ")" ) == 0 ) break;
				}

				// �G���[������������ DLL �ɂ̂ݏo�͂���
				if( i == -1 )
				{
					goto CPPOUTONLY;
				}

				// �����̐���ۑ�
				Func.ParameterNum = i;
			}
			ttxt.getstr( Str );
		}

		// �����̊֐����������Ă��Ȃ��ꍇ�� dx_�֐��� �� DLL �̊֐����ɂ���
		if( funccount == 1 )
		{
			sprintf( DllFuncName, "dx_%s", Func.Name );
		}
		else
		// �Q�ڈȏ�̏ꍇ�� dx_�֐���_���ڂ� �� DLL �̊֐����ɂ���
		{
			sprintf( DllFuncName, "dx_%s_%d", Func.Name, funccount );
		}

		// unsafe �̏ꍇ�̓}�N�����o�͂���
		if( unsafe )
		{
			fprintf( cs, "#if DX_USE_UNSAFE\n" );
		}

		// DLL �ւ̏o�͊֐����X�g�ɏo��
		fprintf2( def, def64, "\t%s\n", DllFuncName ) ;

		// C# �p�̊֐��錾�o��
		for( l = 0; l < 2; l++ )
		{
			fprintf( cs, "\t\t[DllImport(\"DxLib%s.dll\", EntryPoint=\"%s\")]\n", l == 0 ? "" : "_x64", DllFuncName );
			fprintf( cs, "\t\textern %sstatic %s %s_%s( ", unsafe ? "unsafe " : "", Func.CSType, DllFuncName, l == 0 ? "x86" : "x64" );

			// �����������o��
			par = Func.Parameter;
			for( i = 0; i < Func.ParameterNum; i ++, par ++ )
			{
				fprintf( cs, "%s %s", par->CSType, par->Name );
				if( i != Func.ParameterNum - 1 )
				{
					fprintf( cs, ", " );
				}
			}
			fprintf( cs, ");\n" );
		}

		// C#�p�֐��o��
		{
			// unsafe �֐��ł��Ȃ� void * �ȊO�� out �����̈���������ꍇ�̓f�t�H���g�������֐��� C# �ŃG�~�����[�g�ł��Ȃ��̂�
			// ���̂悤�Ȉ��������邩�ǂ������ׂ�
			par = Func.Parameter ;
			for( l = 0 ; l < Func.ParameterNum ; l ++, par ++ )
			{
				if( par->ValidDefault && par->Out )
				{
					if( unsafe == false || strcmp( par->CSType, "void * " ) != 0 )
					{
						// ��������f�t�H���g�����𖳌��ɂ���
						Func.DefaultNum = 0;
						par = Func.Parameter ;
						for( k = 0 ; k < Func.ParameterNum ; k ++, par ++ )
						{
							par->ValidDefault = false ;
						}
						break ;
					}
				}
			}

			// �f�t�H���g���������̏o�͂ƁA�f�t�H���g�������������̐������֐����쐬����K�v������̂ł��̃��[�v
			for( j = 0 ; j <= Func.DefaultNum ; j ++ )
			{
				// �߂�l�̌^�Ɗ֐����̏o��
				fprintf( cs, "\t\tpublic %sstatic %s %s( ", unsafe ? "unsafe " : "", Func.CSType, Func.Name );

				// �p�����[�^��`�̏o��
				par = Func.Parameter;
				k = 0;
				for( i = 0; i < Func.ParameterNum; i ++, par ++ )
				{
					// �f�t�H���g�������g�p����ꍇ�͂���ȍ~�̈����̏o�͍͂s��Ȃ�
					if( par->ValidDefault )
					{
						k ++ ;
						if( k > j ) break;
					}

					// �����ƈ����̊Ԃ̏o��
					if( i != 0 )
					{
						fprintf( cs, ", " );
					}

					// �����̌^�ƈ������̏o��
					fprintf( cs, "%s %s", par->CSType, par->Name );
				}
				fprintf( cs, ")\n" );

				// �֐��̒��g���o��
				fprintf( cs, "\t\t{\n" );

				// out �^�C�v�̃f�t�H���g�����́A���[�J���ϐ��ŏo�͂��󂯎��悤�ɂ���
				par = Func.Parameter;
				k = 0;
				for( i = 0; i < Func.ParameterNum; i ++, par ++ )
				{
					if( par->ValidDefault == 0 ) continue ;
					if( k >= j && par->Out && strcmp( par->CSType, "string " ) != 0 && strcmp( par->CSType, "void * " ) != 0 )
					{
						fprintf( cs, "\t\t\t%s%s;\n", par->CSType[0] == 'o' && par->CSType[1] == 'u' && par->CSType[2] == 't' && par->CSType[3] == ' ' ? &par->CSType[ 4 ] : par->CSType, par->Name ) ;
					}
					k ++ ;
				}

				for( l = 0; l < 2; l++ )
				{
					if( l == 0 )
					{
						// 64bit��32bit���̕���
						fprintf( cs, "\t\t\tif( System.IntPtr.Size == 4 )\n" );
						fprintf( cs, "\t\t\t{\n" );
					}

					// void �^�̏ꍇ�� return ���o�͂��Ȃ�
					if( Func.Void == false )
					{
						fprintf( cs, "\t\t\t\treturn " );
					}
					else
					{
						fprintf( cs, "\t\t\t\t" );
					}

					// DLL �̊֐������o��
					fprintf( cs, "%s%s( ", DllFuncName, l == 0 ? "_x86" : "_x64" ) ;

					// �������o��
					par = Func.Parameter;
					k = 0;
					for( i = 0; i < Func.ParameterNum; i ++, par ++ )
					{
						// ��؂�� , ���o��
						if( i != 0 )
						{
							fprintf( cs, ", " );
						}

						// �f�t�H���g���������邩�ǂ����ŕ���
						if( par->ValidDefault && k >= j )
						{
							// �f�t�H���g������ NULL ���ǂ����ŏ����𕪊�
							if( strcmp( par->Default, "NULL" ) == 0 )
							{
								// string �� void * �̏ꍇ�� null ���o��
								if( strcmp( par->CSType, "string " ) == 0 || strcmp( par->CSType, "void * " ) == 0 )
								{
									fprintf( cs, "null " );
								}
								else
								// ����ȊO�̏ꍇ�͈������Ɠ����̃��[�J���ϐ�����`����Ă���̂ł�����g��
								if( unsafe )
								{
									fprintf( cs, "%s ", par->Name ) ;
								}
								else
								{
									fprintf( cs, "out %s ", par->Name ) ;
								}
							}
							else
							{
								// NULL �ȊO�̏ꍇ�̓|�C���^�n�ł͂Ȃ��̂ŁA�f�t�H���g���l�����̂܂܏o�͂���
								fprintf( cs, "%s ", par->Default );
							}
						}
						else
						{
							// �f�t�H���g�����������ꍇ

							// out ���������邩�ǂ����ŏ����𕪊�
							if( par->Out )
							{
								// out �����ł� System.Text.StringBuilder �� unsafe �̏ꍇ�͂��̂܂ܕϐ������o��
								if( strncmp( par->CSType, "System.Text.StringBuilder", strlen( "System.Text.StringBuilder" ) ) == 0 || unsafe )
								{
									fprintf( cs, "%s ", par->Name );
								}
								else
								// ����ȊO�̏ꍇ�� out ��t����
								{
									fprintf( cs, "out %s ", par->Name );
								}
							}
							else
							{
								// out �����������ꍇ�͕��ʂɏo��
								fprintf( cs, "%s ", par->Name );
							}
						}

						// �f�t�H���g�������������ꍇ�̓J�E���^���C���N�������g
						if( par->ValidDefault )
							k ++ ;
					}
					fprintf( cs, ");\n" );

					if( l == 0 )
					{
						fprintf( cs, "\t\t\t}\n" );
						fprintf( cs, "\t\t\telse\n" );
						fprintf( cs, "\t\t\t{\n" );
					}
					else
					{
						fprintf( cs, "\t\t\t}\n" );
					}
				}

				fprintf( cs, "\t\t}\n" );
			}
		}

		// DLL�p�֐��o��
		{
			// GetColor �����ʏ���
			if( strcmp( Func.CPPType, "DWORD " ) == 0 && strcmp( Func.Name, "GetColor" ) == 0 )
			{
				fprintf( hd, "int __stdcall dx_GetColor( " );
				fprintf( sc, "int __stdcall dx_GetColor( " );
			}
			else
			{
				// �����̊֐�������ꍇ�͊֐����̌��ɉ��ڂ��̔ԍ����o�͂���
				if( funccount == 1 )
				{
					fprintf( hd, "%s __stdcall dx_%s( ", Func.CPPType, Func.Name );
					fprintf( sc, "%s __stdcall dx_%s( ", Func.CPPType, Func.Name );
				}
				else
				{
					fprintf( hd, "%s __stdcall dx_%s_%d( ", Func.CPPType, Func.Name, funccount );
					fprintf( sc, "%s __stdcall dx_%s_%d( ", Func.CPPType, Func.Name, funccount );
				}
			}

			// �����̏o��
			{
				// �����������ꍇ�� void ���o�͂��ďI��
				if( Func.ParameterNum == 0 )
				{
					fprintf( hd, "void" );
					fprintf( sc, "void" );
				}
				else
				{
					// �������o��
					par = Func.Parameter;
					for( i = 0; i < Func.ParameterNum; i ++, par ++ )
					{
						// ��؂蕶���̏o��
						if( i != 0 )
						{
							fprintf( hd, ", " );
							fprintf( sc, ", " );
						}

						// �����̌^�ƈ��������o��
						fprintf( hd, "%s %s", par->CPPType, par->Name );
						fprintf( sc, "%s %s", par->CPPType, par->Name );

						// �f�t�H���g����������ꍇ�͂�����o��
						if( par->ValidDefault )
						{
							fprintf( hd, " = %s", par->Default );
						}
					}
				}
				fprintf( hd, ");\n" );
				fprintf( sc, ")\n" );
			}

			// �֐��̒��g���o��
			{
				fprintf( sc, "{\n" );

				// �߂�l�� void �^�ȊO�̏ꍇ�� return ���o��
				if( Func.Void )
				{
					fprintf( sc, "\t" );
				}
				else
				{
					fprintf( sc, "\treturn " );
				}

				// �֐����̏o��
				fprintf( sc, "%s( ", Func.Name );

				// �����̏o��
				par = Func.Parameter;
				for( i = 0; i < Func.ParameterNum; i ++, par ++ )
				{
					if( i != 0 )
					{
						fprintf( sc, ", " );
					}
					fprintf( sc, "%s", par->Name );
				}
				fprintf( sc, ");\n" );
				fprintf( sc, "}\n" );
			}
		}

		// unsafe �̏ꍇ�̓}�N���̏I�[���o�͂���
		if( unsafe )
		{
			fprintf( cs, "#endif\n" );
		}
		fprintf( cs, "\n" );

		continue ;

		// DLL �ɂ̂ݏo�͂���ꍇ
CPPOUTONLY:
		ttxt.move( FuncStart ) ;

		// �֐��̖߂�l�� void �^���ǂ������Z�b�g
		ttxt.getstr( Str ) ;
		Func.Void = strcmp( Str, "void" ) == 0 ? 1 : 0 ;

		// �֐��̖߂�l�̌^�Ɗ֐������o��
		{
			fprintf( hd, "%s ", Str ) ;
			fprintf( sc, "%s ", Str ) ;

			// �֐���������Ƃ���܂ŕ�������o��
			for(;;)
			{
				ttxt.getstr( Str ) ;
				if( Str[ 0 ] == '*' ) Func.Void = 0 ;
				if( ttxt.nextchar() == '(' ) break ;
				fprintf( hd, "%s ", Str ) ;
				fprintf( sc, "%s ", Str ) ;
			}
		}

		// �֐�����ۑ�
		strcpy( Func.Name, Str ) ;

		// �֐����o�^���܂���������֐�����o�^
		if( funccount == 0x0ccccccc )
		{
			funccount = AddFunctionName( Func.Name ) ;
		}

		// DLL �ł̊֐����̍쐬
		if( funccount == 1 )
		{
			sprintf( Str2, "dx_%s", Func.Name ) ;
		}
		else
		{
			sprintf( Str2, "dx_%s_%d", Func.Name, funccount ) ;
		}

		// �֐������o��
		fprintf2( def, def64, "\t%s\n", Str2 ) ;
		fprintf( hd, "__stdcall %s( ", Str2 ) ;
		fprintf( sc, "__stdcall %s( ", Str2 ) ;

		// �����̏o��
		{
			Func.ParameterNum = 0 ;
			ttxt.getstr( Str ) ;
			ttxt.getstr( Str ) ;

			// �����������ꍇ�� void �Ƃ����o�͂���
			if( strcmp( Str, "void" ) == 0 && ttxt.nextchar() != '*' && ttxt.nextchar() != '(' )
			{
				ttxt.getstr( Str ) ;
				ttxt.getstr( Str ) ;
				fprintf( hd, "void )" ) ;
				fprintf( sc, "void )" ) ;
			}
			else
			{
				for(;;)
				{
					ttxt.getstr( Str2 ) ;

					// �R�߂� * �̏ꍇ�͏����𕪊�
					if( ttxt.nextchar() == '*' )
					{
						ttxt.getstr( Str3 ) ;

						// �Q�ڂ� ( �̏ꍇ�͈����͊֐��|�C���^
						if( Str2[ 0 ] == '(' )
						{
							// �֐��|�C���^�̏ꍇ�͂S�ڂ�������
							ttxt.getstr( Func.Parameter[ Func.ParameterNum ].Name ) ;

							// ) �̎擾
							ttxt.getstr( Str2 ) ;

							// �֐��|�C���^�̈����������̏o��
							fprintf( hd, "%s ( *%s )", Str, Func.Parameter[ Func.ParameterNum ].Name ) ;
							fprintf( sc, "%s ( *%s )", Str, Func.Parameter[ Func.ParameterNum ].Name ) ;

							// �֐��|�C���^�̊֐��̈������o�͂���
							for(;;)
							{
								ttxt.getstr( Str ) ;
								if( Str[0] == ')' ) break ;
								fprintf( hd, "%s ", Str ) ;
								fprintf( sc, "%s ", Str ) ;
							}
							fprintf( hd, "%s ", Str ) ;
							fprintf( sc, "%s ", Str ) ;
						}
						else
						{
							// �֐��|�C���^�ȊO�̏ꍇ�͕��ʂ̃|�C���^�����Ƃ��ďo��
							ttxt.getstr( Func.Parameter[ Func.ParameterNum ].Name ) ;
							fprintf( hd, "%s %s * %s ", Str, Str2, Func.Parameter[ Func.ParameterNum ].Name ) ;
							fprintf( sc, "%s %s * %s ", Str, Str2, Func.Parameter[ Func.ParameterNum ].Name ) ;
						}
					}
					else
					// �|�C���^�� unsigned int �̏ꍇ�͕��ʂɏo��
					if( Str2[ 0 ] == '*' || strcmp( Str2, "int" ) == 0 )
					{
						ttxt.getstr( Func.Parameter[ Func.ParameterNum ].Name ) ;
						fprintf( hd, "%s %s %s ", Str, Str2, Func.Parameter[ Func.ParameterNum ].Name ) ;
						fprintf( sc, "%s %s %s ", Str, Str2, Func.Parameter[ Func.ParameterNum ].Name ) ;
					}
					else
					// ����ȊO�̏ꍇ�����ʂɏo��
					{
						strcpy( Func.Parameter[ Func.ParameterNum ].Name, Str2 ) ;
						fprintf( hd, "%s %s ", Str, Func.Parameter[ Func.ParameterNum ].Name ) ;
						fprintf( sc, "%s %s ", Str, Func.Parameter[ Func.ParameterNum ].Name ) ;
					}
					Func.ParameterNum ++ ;

					ttxt.getstr( Str ) ;
					if( Str[ 0 ] == '=' )
					{
						// �f�t�H���g����������ꍇ�͋�؂蕶��������܂ŏo��
						for(;;)
						{
							fprintf( hd, "%s ", Str ) ;
							ttxt.getstr( Str ) ;
							if( Str[ 0 ] == ',' || Str[ 0 ] == ')' )
								break ;
						}
						fprintf( hd, "%s ", Str ) ;
						fprintf( sc, "%s ", Str ) ;
					}
					else
					{
						fprintf( hd, "%s ", Str ) ;
						fprintf( sc, "%s ", Str ) ;
					}

					// ) ��������I��
					if( Str[ 0 ] == ')' ) break ;
					ttxt.getstr( Str ) ;
				}
				ttxt.getstr( Str ) ;
			}
			fprintf( hd, ";\n" ) ;
			fprintf( sc, "\n" ) ;
		}

		// �֐��̒��g�̏o��
		{
			fprintf( sc, "{\n" ) ;

			// �߂�l�̌^�� void �ȊO�̏ꍇ�� return ��t����
			if( Func.Void )
			{
				fprintf( sc, "\t %s( ", Func.Name ) ;
			}
			else
			{
				fprintf( sc, "\treturn %s( ", Func.Name ) ;
			}

			// �����̏o��
			for( i = 0 ; i < Func.ParameterNum ; i ++ )
			{
				fprintf( sc, "%s ", Func.Parameter[ i ].Name ) ;
				if( i != Func.ParameterNum - 1 )
				{
					fprintf( sc, ", " ) ;
				}
			}
			fprintf( sc, " ) ;\n" ) ;
			fprintf( sc, "}\n" ) ;
		}
	}
	fprintf( cs, "\n\n" );
}

// �蓮�֐���`�̏o�͂��y�ɂ���֐�
void OutputCSFunctionDefine( FILE *cs, char *FuncName, char *CSRet, char *CSParamDefine, bool Unsafe )
{
	fprintf( cs, "\t\t[DllImport(\"DxLib.dll\",     EntryPoint=\"%s\")]\n", FuncName );
	fprintf( cs, "\t\textern %sstatic %s %s_x86%s;\n", Unsafe ? "unsafe " : "", CSRet, FuncName, CSParamDefine );
	fprintf( cs, "\t\t[DllImport(\"DxLib_x64.dll\", EntryPoint=\"%s\")]\n", FuncName );
	fprintf( cs, "\t\textern %sstatic %s %s_x64%s;\n", Unsafe ? "unsafe " : "", CSRet, FuncName, CSParamDefine );
}

// �蓮�֐��R�[�h�̏o�͂��y�ɂ���֐�
void OutputCSFunctionCode( FILE *cs, char *FuncCode1, char *FuncCode2 )
{
	fprintf( cs, "\t\t{\n" );
	fprintf( cs, "\t\t\tif( System.IntPtr.Size == 4 )\n" );
	fprintf( cs, "\t\t\t{\n" );
	fprintf( cs, "\t\t\t\t%s_x86%s;\n", FuncCode1, FuncCode2 );
	fprintf( cs, "\t\t\t}\n" );
	fprintf( cs, "\t\t\telse\n" );
	fprintf( cs, "\t\t\t{\n" );
	fprintf( cs, "\t\t\t\t%s_x64%s;\n", FuncCode1, FuncCode2 );
	fprintf( cs, "\t\t\t}\n" );
	fprintf( cs, "\t\t}\n" );
}

// �蓮�֐��R�[�h�̏o�͂��y�ɂ���֐�
void OutputCSFunctionCode2( FILE *cs, char *FuncCode1, char *FuncCode2 )
{
	fprintf( cs, "\t\t\tif( System.IntPtr.Size == 4 )\n" );
	fprintf( cs, "\t\t\t{\n" );
	fprintf( cs, "\t\t\t\t%s_x86%s;\n", FuncCode1, FuncCode2 );
	fprintf( cs, "\t\t\t}\n" );
	fprintf( cs, "\t\t\telse\n" );
	fprintf( cs, "\t\t\t{\n" );
	fprintf( cs, "\t\t\t\t%s_x64%s;\n", FuncCode1, FuncCode2 );
	fprintf( cs, "\t\t\t}\n" );
}

// main�֐�
void main( int argc, char **argv )
{
	FILE *hd, *sc, *cs, *def, *def64;
	
	// �p�����[�^�� 2 ����������w���v���o�͂��ďI��
	if( argc != 3 )
	{
		printf( "MakeDxLibDLLSource.exe DxLibHeaderPath DxDirectXHeaderPath\n" );
		return;
	}

	// DxLib.h �̓ǂݍ���
	if( dhtxt.load( argv[1] ) == false )
	{
		printf( "DxLib.h �̃I�[�v���Ɏ��s���܂���\n" ) ;
		return;
	}

	// DxDirectX.h �̓ǂݍ���
	if( dxtxt.load( argv[2] ) == false )
	{
		printf( "DxDirectXb.h �̃I�[�v���Ɏ��s���܂���\n" ) ;
		return;
	}

	// �Ӗ��̂����؂�ɃX�y�[�X��������������̍쐬
	CreateTokenStr();

	// �o�͐�̃t�@�C�����J��
	hd = fopen( "DxDLL.h", "wt" );
	sc = fopen( "DxDLL.cpp", "wt" );
	cs = fopen( "DxDLL.cs", "wt" );
	def = fopen( "DxDLL.def", "wt" ) ;
	def64 = fopen( "DxDLLx64.def", "wt" ) ;

	// �擪�����̏o��
	fprintf( def, "LIBRARY DxLib\n\nEXPORTS\n" ) ;
	fprintf( def64, "LIBRARY DxLib_x64\n\nEXPORTS\n" ) ;

	fprintf( hd, "#include \"DxLib.h\"\n" );
	fprintf( hd, "extern \"C\"\n{\n " );
	fprintf( sc, "#include \"DxDLL.h\"\n" );
	fprintf( sc, "#define DX_CHAR char\n" );

	fprintf( cs, "using System.Runtime.InteropServices;\n" );
	fprintf( cs, "namespace DxLibDLL\n" );
	fprintf( cs, "{\n" );
	fprintf( cs, "\tpublic static class DX\n" );
	fprintf( cs, "\t{\n" );

	// �萔��`�� C# �̃t�@�C���ɏo�͂���
	OutputDefineStr( cs ) ;

	// �\���̕������o�͂���
	OutputStructStr( cs ) ;

	// �֐��������o�͂���
	OutputFunctionStr( cs, sc, def, def64, hd ) ;

	// �I�[�������o��
	fprintf( hd, "}\n" );
	fprintf( cs, "\t}\n" );
	fprintf( cs, "}\n" );

	// �t�@�C�������
	fclose( hd );
	fclose( sc );
	fclose( cs );
	fclose( def ) ;
	fclose( def64 ) ;

	// DxLib.h �̉��
	dhtxt.release() ;

	// DxDirectX.h �̉��
	dxtxt.release() ;
}




