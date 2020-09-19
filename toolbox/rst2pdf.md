# rst转PDF
## 安装
```bash
easy_install rst2pdf
```
## 配置rst2pdf
找到conf.py文件

```bash
extensions = [
    'sphinx.ext.autodoc',
    'rst2pdf.pdfbuilder'
]
```
然后在conf.py中拷入PDF相关的配置：

```bash
pdf_documents = [
    ('index', u'HanLP Handbook', u'HanLP Handbook', u'hankcs'),
]
 
# A comma-separated list of custom stylesheets. Example:
pdf_stylesheets = ['a3','zh_CN']
 
# Create a compressed PDF
# Use True/False or 1/0
# Example: compressed=True
#pdf_compressed = False
 
# A colon-separated list of folders to search for fonts. Example:
pdf_font_path = ['C:\\Windows\\Fonts']
 
# Language to be used for hyphenation support
pdf_language = "zh_CN"
 
# Mode for literal blocks wider than the frame. Can be
# overflow, shrink or truncate
pdf_fit_mode = "shrink"
 
# Section level that forces a break page.
# For example: 1 means top-level sections start in a new page
# 0 means disabled
#pdf_break_level = 0
 
# When a section starts in a new page, force it to be 'even', 'odd',
# or just use 'any'
#pdf_breakside = 'any'
 
# Insert footnotes where they are defined instead of
# at the end.
#pdf_inline_footnotes = True
 
# verbosity level. 0 1 or 2
#pdf_verbosity = 0
 
# If false, no index is generated.
#pdf_use_index = True
 
# If false, no modindex is generated.
#pdf_use_modindex = True
 
# If false, no coverpage is generated.
#pdf_use_coverpage = True
 
# Documents to append as an appendix to all manuals.
#pdf_appendices = []
 
# Enable experimental feature to split table cells. Use it
# if you get "DelayedTable too big" errors
#pdf_splittables = False
 
# Set the default DPI for images
#pdf_default_dpi = 72
 
# Enable rst2pdf extension modules (default is only vectorpdf)
# you need vectorpdf if you want to use sphinx's graphviz support
#pdf_extensions = ['vectorpdf']
 
# Page template name for "regular" pages
#pdf_page_template = 'cutePage'
 
# Show Table Of Contents at the beginning?
# pdf_use_toc = False
 
# How many levels deep should the table of contents be?
pdf_toc_depth = 2
 
# Add section number to section references
pdf_use_numbered_links = False
 
# Background images fitting mode
pdf_fit_background_mode = 'scale'
```
## 样式修改
在项目根目录下创建一个zh_CN.json，写入：
```bash
{
  "embeddedFonts": [
    "simsun.ttc"
  ],
  "fontsAlias": {
    "stdFont": "simsun",
    "stdBold": "simsun",
    "stdItalic": "simsun",
    "stdBoldItalic": "simsun",
    "stdMono": "simsun",
    "stdMonoBold": "simsun",
    "stdMonoItalic": "simsun",
    "stdMonoBoldItalic": "simsun",
    "stdSans": "simsun",
    "stdSansBold": "simsun",
    "stdSansItalic": "simsun",
    "stdSansBoldItalic": "simsun"
  },
  "styles": [
    [
      "base",
      {
        "wordWrap": "CJK"
      }
    ],
    [
      "literal",
      {
        "wordWrap": "None"
      }
    ]
  ]
}
```
## 配置编译脚本
在make.bat中加入：
```bash
if "%1" == "pdf" (
   %SPHINXBUILD% -b pdf %ALLSPHINXOPTS% %BUILDDIR%/pdf
   if errorlevel 1 exit /b 1
   echo.
   echo.Build finished. The pdf files are in %BUILDDIR%/pdf.
   goto end
)
```
## 输出pdf
执行命令
```bash
 .\make.bat pdf
```
## 去除空白页

将pdf_toc_depth调小一点，或者干脆不生成目录，pdf_use_toc = False。
 
