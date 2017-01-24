﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#if defined(_WIN32) || defined (_WIN64)
#else
#endif

#include "../../DesktopEditor/common/Types.h"
#include "../../Common/DocxFormat/Source/Base/Types_32.h"

#include <vector>
#include <map>
#include <string>

#define MAX_STACK_SIZE 1024

class COfficeFontPicker;

namespace OOX
{
	class WritingElement;
}
namespace NSCommon
{
	template<typename Type> class smart_ptr;
	class nullable_string;
	class nullable_bool;
	class nullable_int;
	class nullable_double;
	class nullable_sizet;
}
namespace PPTX
{
	class FileContainer;
	class CCommonRels;
}
namespace BinDocxRW
{
	class CDocxSerializer;
}
namespace NSShapeImageGen
{
	class CImageManager;
}
namespace NSFontCutter
{
	class CFontDstManager;
}

namespace NSBinPptxRW
{
	class CStringWriter;
	class CCommonWriter;
	class CSeekTableEntry;
	class CImageManager2Info
	{
	public:
		std::wstring m_sImagePath;
		std::wstring m_sOlePath;
		std::wstring m_sFilepathBin;
		std::wstring m_sFilepathImg;
	};
	class CRelsGeneratorInfo
	{
	public:
		int m_nImageRId;
		int m_nOleRId;
		std::wstring m_sFilepathBin;
		std::wstring m_sFilepathImg;
		CRelsGeneratorInfo()
		{
			m_nImageRId = -1;
			m_nOleRId = -1;
		}
	};
	class CMasterSlideInfo
	{
	public:
		CMasterSlideInfo();

	public:
		_INT32 m_lThemeIndex;
		std::string m_strImageBase64;

        std::vector<LONG>       m_arLayoutIndexes;
        std::vector<std::string>   m_arLayoutImagesBase64;
	};

	class CCommonWriter
	{
	public:
        std::map<size_t, LONG> themes;
        std::map<size_t, LONG> slideMasters;
        std::map<size_t, LONG> slides;
        std::map<size_t, LONG> layouts;
        std::map<size_t, LONG> notes;
        std::map<size_t, LONG> notesMasters;

        std::vector<CMasterSlideInfo>	m_oRels;
        std::vector<LONG>				m_oSlide_Layout_Rels;

		NSShapeImageGen::CImageManager*	m_pImageManager;
		
		//NSFontCutter::CFontDstManager	m_oFontPicker;

		NSFontCutter::CFontDstManager*	m_pNativePicker;
		COfficeFontPicker*				m_pFontPicker;
		bool m_bDeleteFontPicker;

	public:
		CCommonWriter();
		~CCommonWriter();

	public:
		void CreateFontPicker(COfficeFontPicker* pPicker);
		void CheckFontPicker();
	};

	class CImageManager2
	{
	private:
        std::map<std::wstring, CImageManager2Info>	m_mapImages;
		_INT32						m_lIndexNextImage;
		_INT32						m_lIndexNextOle;
		std::wstring						m_strDstMedia;
		std::wstring						m_strDstEmbed;
	public:
        bool						m_bIsWord;

	public:
		CImageManager2();
		~CImageManager2();
		void Clear();
		void SetDstMedia(const std::wstring& strDst);
		std::wstring GetDstMedia();
		void SetDstEmbed(const std::wstring& strDst);
		std::wstring GetDstEmbed();
		int IsDisplayedImage(const std::wstring& strInput);

	public:
		template <typename T>
		void Serialize(T* pWriter)
		{
			pWriter->WriteBYTE(m_bIsWord ? 1 : 0);
			pWriter->WriteINT(m_lIndexNextImage);
			pWriter->WriteINT(m_lIndexNextOle);
			pWriter->WriteString(m_strDstMedia);
			
            int lCount = (int)m_mapImages.size();
			pWriter->WriteINT(lCount);

            //for (std::map<std::wstring, std::wstring>::const_iterator pPair = m_mapImages.begin(); pPair != m_mapImages.end(); ++pPair)
            //{
            //    pWriter->WriteString(pPair->first);
            //    pWriter->WriteString(pPair->second);
            //}
		}

		template <typename T>
		void Deserialize(T* pReader)
		{
            m_bIsWord = ((true == pReader->GetBool()) ? true : false);
			m_lIndexNextImage = pReader->GetLong();
			m_lIndexNextOle = pReader->GetLong();
			m_strDstMedia = pReader->GetString2();

			m_mapImages.clear();
			_INT32 lCount = pReader->GetLong();

			//for (_INT32 i = 0; i < lCount; ++i)
			//{
			//	std::wstring s1 = pReader->GetString2();
			//	std::wstring s2 = pReader->GetString2();

			//	m_mapImages [s1] = s2;
			//}
		}

	public:
		CImageManager2Info GenerateImage(const std::wstring& strInput, const std::wstring& oleData, std::wstring strBase64Image);
		CImageManager2Info GenerateImageExec(const std::wstring& strInput, const std::wstring& strExts, const std::wstring& strOleImage, const std::wstring& oleData);

		void SaveImageAsPng(const std::wstring& strFileSrc, const std::wstring& strFileDst);

		void SaveImageAsJPG(const std::wstring& strFileSrc, const std::wstring& strFileDst);

		bool IsNeedDownload(const std::wstring& strFile);
		CImageManager2Info DownloadImage(const std::wstring& strFile);
		std::wstring DownloadImageExec(const std::wstring& strFile);
		bool WriteOleData(const std::wstring& sFilePath, const std::wstring& sData);
	};

	class CBinaryFileWriter
	{
	public:
		class CSeekTableEntry
		{
		public:
			_INT32 Type;
			_INT32 SeekPos;

		public:
			CSeekTableEntry();
		};

		CCommonWriter* m_pCommon;
		std::wstring m_strMainFolder;

		NSCommon::smart_ptr<PPTX::CCommonRels>	*	m_pCommonRels;
		BinDocxRW::CDocxSerializer *				m_pMainDocument;

		NSCommon::smart_ptr<PPTX::FileContainer>* ThemeDoc;
		NSCommon::smart_ptr<OOX::WritingElement>* ClrMapDoc;
	private:
		BYTE*		m_pStreamData;
		BYTE*		m_pStreamCur;
		_UINT32		m_lSize;

		_UINT32		m_lPosition;
		_UINT32		m_arStack[MAX_STACK_SIZE];
		_UINT32		m_lStackPosition;
		
		std::vector<CSeekTableEntry> m_arMainTables;

	public:
		_INT32 m_lWidthCurShape;
		_INT32 m_lHeightCurShape;

	public:
		BYTE*				GetBuffer();
		_UINT32	GetPosition();
		void				SetPosition(const _UINT32& lPosition);
		void				Skip(const _UINT32& lSize);

		double GetWidthMM();
		double GetHeightMM();
		void ClearShapeCurSizes();

		// -------------------- stream simple types -----------------------
	public:
		void Clear();

		void SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc);

		void ClearNoAttack();

		void CheckBufferSize(_UINT32 lPlus);
		
		void WriteBYTE	(const BYTE& lValue);
		void WriteSBYTE	(const signed char& lValue);
		void WriteBOOL	(const bool& bValue);
		void WriteUSHORT(const _UINT16& lValue);
		
		void WriteULONG	(const _UINT32& lValue);
		void WriteLONG	(const _INT32& lValue);
		void WriteINT	(const _INT32& lValue);
		
		void WriteDouble	(const double& dValue);
		void WriteDoubleReal(const double& dValue);
		
		void WriteBYTEArray	(const BYTE* pBuffer, size_t len);
		void WriteStringA	(std::string& sBuffer);
		
		void WriteStringW	(std::wstring& sBuffer);
		void WriteStringW	(const std::wstring& sBuffer);
		
		void WriteStringW2	(std::wstring& sBuffer);
		
		void WriteStringW3	(std::wstring& sBuffer);
		void WriteStringW3	(const std::wstring& sBuffer);
		
		void WriteStringW4	(const std::wstring& sBuffer);
		// --------------------------------------------------------
		void WriteLONG64	(const _INT64& lValue);
		void WriteDouble64	(const double& dValue);
		// --------------------------------------------------------

	public: 
		CBinaryFileWriter();
		~CBinaryFileWriter();

		void StartRecord(_INT32 lType);
		void EndRecord();

		void StartMainRecord(_INT32 lType);
		void WriteReserved(size_t lCount);

		void WriteMainPart();

	public:
		
		void WriteString1	(int type, const std::wstring& val);
		void WriteString2	(int type, const NSCommon::nullable_string& val);
		void WriteString	(const std::wstring& val);

		void WriteString1Data(int type, const WCHAR* pData, _UINT32 len);

		void WriteBool1(int type, const bool& val);
		void WriteBool2(int type, const NSCommon::nullable_bool& val);

		void WriteInt1(int type, const int& val);
		void WriteInt2(int type, const NSCommon::nullable_int& val);

		void WriteDouble1(int type, const double& val);
		void WriteDouble2(int type, const NSCommon::nullable_double& val);

		void WriteSize_t1(int type, const size_t& val);
		void WriteSize_t2(int type, const NSCommon::nullable_sizet& val);

		template<typename T>
		void WriteLimit1(int type, const T& val)
		{
			BYTE bType = (BYTE)type;
			WriteBYTE(bType);
			WriteBYTE(val.GetBYTECode());
		}
		template<typename T>
		void WriteLimit2(int type, const T& val)
		{
			if (val.is_init())
				WriteLimit1(type, *val);
		}

		template<typename T>
		void WriteRecord1(int type, const T& val)
		{
			StartRecord(type);
			val.toPPTY(this);
			EndRecord();
		}
		template<typename T>
		void WriteRecord2(int type, const T& val)
		{
			if (val.is_init())
			{
				StartRecord(type);
				val->toPPTY(this);
				EndRecord();
			}
		}

		template<typename T>
		void WriteRecordArray(int type, int subtype, const std::vector<T>& val)
		{
			StartRecord(type);

			_UINT32 len = (_UINT32)val.size();
			WriteULONG(len);

			for (_UINT32 i = 0; i < len; ++i)
				WriteRecord1(subtype, val[i]);

			EndRecord();
		}

		void GetBase64File(const std::wstring& sFile, std::string& strDst64);

		void WriteTheme64(_INT32 lIndex, const std::wstring& sFile);

		void WriteLayoutTheme64(_INT32 lIndexTheme, _INT32 lIndexLayout, const std::wstring& sFile);

		std::wstring GetFolderForGenerateImages();

		// embedded fonts
		void WriteEmbeddedFonts();

		// serialize ImageManagers
        bool Serialize(NSBinPptxRW::CImageManager2* pManager, BYTE **ppArray, size_t& szCount);
        bool Serialize(NSShapeImageGen::CImageManager* pManager, BYTE **ppArray, size_t& szCount);

        bool GetSafearray(BYTE **ppArray, size_t& szCount);
	private:
		_INT32 _WriteString(const WCHAR* sBuffer, _UINT32 lCount);
		void _WriteStringWithLength(const WCHAR* sBuffer, _UINT32 lCount, bool bByte);
	};
	class CSlideMasterInfo
	{
	public:
		_INT32			m_lThemeIndex;
		std::vector<LONG> m_arLayouts;

	public:
		CSlideMasterInfo();
		CSlideMasterInfo(const CSlideMasterInfo& oSrc);
	};

	class CRelsGenerator
	{
	private:
		CStringWriter* m_pWriter;
		int									m_lNextRelsID;
		std::map<std::wstring, CRelsGeneratorInfo>				m_mapImages;

		std::map<std::wstring, int>				m_mapLinks;

	public:
		CImageManager2*						m_pManager;

	public:
		CRelsGenerator(CImageManager2* pManager = NULL);
		~CRelsGenerator();
		void Clear();

		void StartRels();

		void StartTheme();

		void StartMaster(int nIndexTheme, const CSlideMasterInfo& oInfo);
		void StartLayout(int nIndexTheme);
		void StartSlide(int nIndexSlide, int nIndexLayout);
		void StartNote(int nIndexSlide);
		void WriteMasters(int nCount);
		void WriteThemes(int nCount);
		void WriteSlides(int nCount);
		void WriteSlideComments(int nComment);
		void EndPresentationRels(const bool& bIsCommentsAuthors);
		int GetNextId();
		void CloseRels();
		void AddRels(const std::wstring& strRels);
		void SaveRels(const std::wstring& strFile);

		CRelsGeneratorInfo WriteImage(const std::wstring& strImagePath, const std::wstring& oleData, std::wstring strBase64Image);
		int WriteChart(int nChartNumber, _INT32 lDocType);

		int WriteRels(const std::wstring& bsType, const std::wstring& bsTarget, const std::wstring& bsTargetMode);

		int WriteHyperlink(const std::wstring& strLink, const bool& bIsActionInit);	
	};

	class CBinaryFileReader
	{
	private:
		BYTE* m_pData;
		_INT32 m_lSize;
		_INT32 m_lPos;
		BYTE* m_pDataCur;

		_INT32 m_lNextId;

	public:
		//CRelsGenerator m_oRels;
		std::wstring m_strFolder;
		std::wstring m_strFolderThemes;
		std::wstring m_strFolderExternalThemes;

		_INT32 m_lChartNumber;
		std::wstring m_strContentTypes;

		BinDocxRW::CDocxSerializer* m_pMainDocument;

		_INT32		m_lDocumentType;

		CRelsGenerator* m_pRels;
		std::vector<CRelsGenerator*> m_stackRels;
		int m_nCurrentRelsStack;
	
	public:
		CBinaryFileReader();
		~CBinaryFileReader();

		void SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc);

	public:
		void Init(BYTE* pData, _INT32 lStart, _INT32 lSize);
		_INT32 GenerateNextId();

	public:

		int Seek(_INT32 _pos);
		int Skip(_INT32 _skip);
		bool Peek(int nSizeToRead);
		
		// 1 bytes
		BYTE GetUChar();
		signed char GetChar();
		bool GetBool();
		BYTE GetUChar_TypeNode();

		// 2 byte
		_UINT16 GetUShort();

		// 4 byte
		_UINT32 GetULong();

		_INT32 GetLong();
		double GetDouble();
		// 8 byte
		_INT64 GetLong64();
		double GetDouble64();
		double GetDoubleReal();
		//String
		std::wstring GetString(_INT32 len);
		std::string GetString1(_INT32 len);
		std::wstring GetString2();
		std::wstring GetString3(_INT32 len);
		std::wstring GetString4(_INT32 len);

        //LPSAFEARRAY GetArray(_INT32 len);
        bool GetArray(BYTE **pBuffer, _INT32 len);

		std::string GetString2A();
		void SkipRecord();

		_INT32 GetPos();

		_INT32 GetSize();

		BYTE* GetData();
		BYTE* GetPointer(int nSize);
	public:
        void Deserialize(NSBinPptxRW::CImageManager2* pManager, BYTE* pData, _INT32 nSize/*, LPSAFEARRAY pArray*/);
        void Deserialize(NSShapeImageGen::CImageManager* pManager, BYTE* pData, _INT32 nSize/*, LPSAFEARRAY pArray*/);
	};
}
