# tohtml_style
1. 先使用 VIM 的 TOhtml 把文本转换成 html
```
vim -c :TOhtml -c :wqa  mycode.cpp
vim -c :TOhtml -c :wqa  mydoc.txt
```

2. 再使用本工具 html_share_style 把编辑好的样式共享给 html文件
```
Usage: html_share_style  <file.html>  <from.html>
```
![](https://raw.githubusercontent.com/hongwenjun/tohtml_style/master/usage.webp)
