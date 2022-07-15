# Планета ZPL
Программа для печати этикеток на сетевых принтерах c поддержкой ZPL
![Скриншот программы](/img/screenshot.png)
## Пример ZPL файла
    ^XA
    ^FO50,60^A0,40^FDHello, world!^FS
    ^FO60,120^BY3^BCN,60,,,,A^FD1234ABC^FS
    ^FO25,25^GB380,200,2^FS
    ^XZ

## Комментарии
* Лучший источник информации по языку ZPL [[pdf](https://www.zebra.com/content/dam/zebra_new_ia/en-us/manuals/printers/common/programming/zpl-zbi2-pm-en.pdf)]
* Программа создавалсь специально для ООО "Адекватные технологии"   
* Единственный принтер, на котором программа была тестирована - [TLP300](https://www.hprt.com/Product/4-inch-Thermal-Transfer-Label-Printer.html)
* По идее, должна работать с любым сетевым принтером с поддержкой ZPL
* Написано в июне 2020 выложено не GitHub в рамках наведения порядка в старых проектах
