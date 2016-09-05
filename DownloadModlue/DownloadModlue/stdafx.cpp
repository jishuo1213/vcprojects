// stdafx.cpp : source file that includes just the standard includes
// DownloadModlue.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

extern const char* MimeTypes[][2] =
{
	{ ".***",	"application/octet-stream" },
	{ ".CSV",	"text/csv" },
	{ ".TSV",		"text/tab-separated-values" },
	{ ".TAB",		"text/tab-separated-values" },
	{ ".HTML",	"text/html" },
	{ ".HTM",	"text/html" },
	{ ".DOC",	"application/msword" },
	{ ".DOCX",	"application/vnd.openxmlformats-officedocument.wordprocessingml.document" },
	{ ".ODS",	"application/x-vnd.oasis.opendocument.spreadsheet" },
	{ ".ODT",	"application/vnd.oasis.opendocument.text" },
	{ ".RTF",		"application/rtf" },
	{ ".SXW",	"application/vnd.sun.xml.writer" },
	{ ".TXT",		"text/plain" },
	{ ".XLS",		"application/vnd.ms-excel" },
	{ ".XLSX",	"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" },
	{ ".PDF",	"application/pdf" },
	{ ".PPT",		"application/vnd.ms-powerpoint" },
	{ ".PPS",	"application/vnd.ms-powerpoint" },
	{ ".PPTX",	"application/vnd.openxmlformats-officedocument.presentationml.presentation" },
	{ ".WMF",	"image/x-wmf" },
	{ ".ATOM",	"application/atom+xml" },
	{ ".XML",	"application/xml" },
	{ ".JSON",	"application/json" },
	{ ".JS",		"application/javascript" },
	{ ".OGG",	"application/ogg" },
	{ ".PS",		"application/postscript" },
	{ ".WOFF",	"application/x-woff" },
	{ ".XHTML","application/xhtml+xml" },
	{ ".XHT",	"application/xhtml+xml" },
	{ ".ZIP",		"application/zip" },
	{ ".GZ",		"application/x-gzip" },
	{ ".RAR",	"application/rar" },
	{ ".RM",		"application/vnd.rn-realmedia" },
	{ ".RMVB",	"application/vnd.rn-realmedia-vbr" },
	{ ".SWF",	"application/x-shockwave-flash" },
	{ ".AU",		"audio/basic" },
	{ ".SND",	"audio/basic" },
	{ ".MID",	"audio/mid" },
	{ ".RMI",		"audio/mid" },
	{ ".MP3",	"audio/mpeg" },
	{ ".AIF",		"audio/x-aiff" },
	{ ".AIFC",	"audio/x-aiff" },
	{ ".AIFF",	"audio/x-aiff" },
	{ ".M3U",	"audio/x-mpegurl" },
	{ ".RA",		"audio/vnd.rn-realaudio" },
	{ ".RAM",	"audio/vnd.rn-realaudio" },
	{ ".WAV",	"audio/x-wave" },
	{ ".WMA",	"audio/x-ms-wma" },
	{ ".M4A",	"audio/x-m4a" },
	{ ".BMP",	"image/bmp" },
	{ ".GIF",		"image/gif" },
	{ ".JPE",		"image/jpeg" },
	{ ".JPEG",	"image/jpeg" },
	{ ".JPG",		"image/jpeg" },
	{ ".JFIF",	"image/jpeg" },
	{ ".PNG",	"image/png" },
	{ ".SVG",	"image/svg+xml" },
	{ ".TIF",		"image/tiff" },
	{ ".TIFF",	"image/tiff" },
	{ ".ICO",	"image/vnd.microsoft.icon" },
	{ ".CSS",	"text/css" },
	{ ".BAS",	"text/plain" },
	{ ".C",		"text/plain" },
	{ ".H",		"text/plain" },
	{ ".RTX",		"text/richtext" },
	{ ".MP2",	"video/mpeg" },
	{ ".MPA",	"video/mpeg" },
	{ ".MPE",	"video/mpeg" },
	{ ".MPEG",	"video/mpeg" },
	{ ".MPG",	"video/mpeg" },
	{ ".MPV2",	"video/mpeg" },
	{ ".MOV",	"video/quicktime" },
	{ ".QT",		"video/quicktime" },
	{ ".LSF",		"video/x-la-asf" },
	{ ".LSX",		"video/x-la-asf" },
	{ ".ASF",		"video/x-ms-asf" },
	{ ".ASR",	"video/x-ms-asf" },
	{ ".ASX",	"video/x-ms-asf" },
	{ ".AVI",		"video/x-msvideo" },
	{ ".3GP",	"video/3gpp" },
	{ ".3GPP",	"video/3gpp" },
	{ ".3G2",	"video/3gpp2" },
	{ ".MOVIE","video/x-sgi-movie" },
	{ ".MP4",	"video/mp4" },
	{ ".WMV",	"video/x-ms-wmv" },
	{ ".WEBM","video/webm" },
	{ ".M4V",	"video/x-m4v" },
	{ ".FLV",		"video/x-flv" }
};
