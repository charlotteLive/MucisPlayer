### Qt设置控件透明的方法
```
listView->setStyleSheet("border-image: \*url();background-color: rgb(14, 154, 154,10);");
```
背景颜色的前三个数代表颜色，最后一个为透明度，设置为0则是全透明。