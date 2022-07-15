#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, "PlanetaDezavid", "PlanetaZPL");
    central_widget = new QWidget(this);
    layout = new QGridLayout(central_widget);
    zpl_edit = new QTextEdit(central_widget);
    zpl_list = new QComboBox(central_widget);
    address_edit = new QLineEdit(central_widget);
    address_label = new QLabel("Адрес принтера", central_widget);
    copies_edit = new QLineEdit(central_widget);
    notifications = new QTextEdit(central_widget);
    print_button = new QPushButton("Печать", central_widget);
    printer = new ZPLPrinter(this);
    print_dialog = new QMessageBox(zpl_edit);
    save_zpl_dialog = new QMessageBox(zpl_edit);
    about_dialog = new QMessageBox(this);
    file_menu = menuBar()->addMenu("Файл");
    view_menu = menuBar()->addMenu("Вид");
    misc_menu = menuBar()->addMenu("?");
    print_act = new QAction("Печать", file_menu);
    save_zpl_act = new QAction("Сохранить этикетку", file_menu);
    save_zpl_as_act = new QAction("Сохранить этикетку как...", file_menu);
    select_zpl_dir_act = new QAction("Выбрать папку с этикетками...", file_menu);
    save_settings_act = new QAction("Сохранить настройки", file_menu);
    quit_act = new QAction("Выйти", file_menu);
    show_address_act = new QAction("Адрес принтера", view_menu);
    show_notifications_act = new QAction("Отладочная консоль", view_menu);
    about_act = new QAction("О программе...", misc_menu);
    zpl_dir_dialog = new QFileDialog(this);
    save_zpl_as_dialog = new QFileDialog(this);
    highlighter = new ZPLHighlighter(zpl_edit->document());

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    zpl_edit->setAcceptRichText(false);
    zpl_edit->setFont(font);


    show_address_act->setCheckable(true);
    show_notifications_act->setCheckable(true);

    save_zpl_act->setShortcut(Qt::CTRL + Qt::Key_S);
    save_zpl_as_act->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    print_act->setShortcut(Qt::CTRL + Qt::Key_P);
    select_zpl_dir_act->setShortcut(Qt::CTRL + Qt::Key_O);
    quit_act->setShortcut(Qt::CTRL + Qt::Key_Q);

    file_menu->addAction(print_act);
    file_menu->addAction(save_zpl_act);
    file_menu->addAction(save_zpl_as_act);
    file_menu->addAction(select_zpl_dir_act);
    file_menu->addAction(save_settings_act);
    file_menu->addAction(quit_act);

    view_menu->addAction(show_address_act);
    view_menu->addAction(show_notifications_act);

    misc_menu->addAction(about_act);

    setMinimumSize(480, 360);
    notifications->setMaximumHeight(80);
    notifications->setReadOnly(true);
    notifications->setLineWrapMode(QTextEdit::WidgetWidth);
    setCentralWidget(central_widget);
    setWindowTitle("Планета ZPL");

    layout->addWidget(address_label, 0, 0);
    layout->addWidget(address_edit, 0, 1, 1, 2);
    layout->addWidget(new QLabel("Файл этикетки", this), 1, 0);
    layout->addWidget(zpl_list, 1, 1, 1, 2);
    layout->addWidget(zpl_edit, 2, 0, 1, 3);
    layout->addWidget(notifications, 3, 0, 1, 3);
    layout->addWidget(new QLabel("Число копий", this), 4, 0);
    layout->addWidget(copies_edit, 4, 1);
    layout->addWidget(print_button, 4, 2);

    print_dialog->addButton("Печать", QMessageBox::AcceptRole);
    print_dialog->addButton("Отмена", QMessageBox::RejectRole);
    print_dialog->setIcon(QMessageBox::Question);
    print_dialog->setWindowTitle("Подтверждение печати");

    save_zpl_dialog->addButton("Да", QMessageBox::AcceptRole);
    save_zpl_dialog->addButton("Отмена", QMessageBox::RejectRole);
    save_zpl_dialog->setIcon(QMessageBox::Question);
    save_zpl_dialog->setWindowTitle("Подтверждение сохранения ZPL файла");

    zpl_dir_dialog->setFileMode(QFileDialog::Directory);
    zpl_dir_dialog->setNameFilter("ZPL файлы (*.zpl)");
    zpl_dir_dialog->setWindowTitle("Выбор папки с ZPL файлами");

    save_zpl_as_dialog->setFileMode(QFileDialog::AnyFile);
    save_zpl_as_dialog->setWindowTitle("Сохранение ZPL файла");
    save_zpl_as_dialog->setNameFilter("ZPL файлы (*.zpl)");
    save_zpl_as_dialog->setDefaultSuffix("zpl");
    save_zpl_as_dialog->setOption(QFileDialog::DontUseNativeDialog);

    about_dialog->addButton("OK", QMessageBox::AcceptRole);
    about_dialog->setIcon(QMessageBox::Information);
    about_dialog->setWindowTitle("О программе");
    about_dialog->setText(QString("<table>"
                                  "<tr><td>Автор: </td><td>Иван Блинов</td></tr>"
                                  "<tr><td>Дата билда: </td><td>%1</td></tr>"
                                  "<tr><td>Время билда: </td><td>%2</td></tr>"
                                  "<tr><td>Версия QT: </td><td>%3</td></tr>"
                                  "</table><br><br>"
                                  "Cпециально для ООО \"Адекватные технологии\"").arg(__DATE__).arg(__TIME__).arg(qVersion()));

    loadSettings();

    QObject::connect(print_button, SIGNAL(clicked()), this, SLOT(print()), Qt::UniqueConnection);
    QObject::connect(printer, SIGNAL(notification(QString, int)), this, SLOT(notify(QString, int)), Qt::UniqueConnection);
    QObject::connect(printer, SIGNAL(readyToPrint(bool)), print_button, SLOT(setEnabled(bool)), Qt::UniqueConnection);
    QObject::connect(printer, SIGNAL(success()), this, SLOT(saveSettings()), Qt::UniqueConnection);
    QObject::connect(print_act, SIGNAL(triggered()), this, SLOT(print()), Qt::UniqueConnection);
    QObject::connect(about_act, SIGNAL(triggered()), about_dialog, SLOT(exec()), Qt::UniqueConnection);
    QObject::connect(save_zpl_act, SIGNAL(triggered()), this, SLOT(saveZpl()), Qt::UniqueConnection);
    QObject::connect(save_zpl_as_act, SIGNAL(triggered()), save_zpl_as_dialog, SLOT(exec()), Qt::UniqueConnection);
    QObject::connect(select_zpl_dir_act, SIGNAL(triggered()), zpl_dir_dialog, SLOT(exec()), Qt::UniqueConnection);
    QObject::connect(zpl_dir_dialog, SIGNAL(fileSelected(QString)), this, SLOT(setZplDir(QString)), Qt::UniqueConnection);
    QObject::connect(save_zpl_as_dialog, SIGNAL(fileSelected(QString)), this, SLOT(saveZplAs(QString)), Qt::UniqueConnection);
    QObject::connect(save_settings_act, SIGNAL(triggered()), this, SLOT(saveSettings()), Qt::UniqueConnection);
    QObject::connect(show_address_act, SIGNAL(toggled(bool)), this, SLOT(showAddress(bool)), Qt::UniqueConnection);
    QObject::connect(show_notifications_act, SIGNAL(toggled(bool)), this, SLOT(showNotifications(bool)), Qt::UniqueConnection);
    QObject::connect(quit_act, SIGNAL(triggered()), this, SIGNAL(quit()), Qt::UniqueConnection);
}

int MainWindow::loadSettings()
{
    settings->sync();
    address_edit->setText(settings->value("address", "127.0.0.1:9100").toString());
    copies_edit->setText(settings->value("copies", 1).toString());
    if (settings->value("window/maximized", false).toBool()) setWindowState(Qt::WindowMaximized);
    resize(settings->value("window/width", 480).toInt(), settings->value("window/height", 360).toInt());

    QRegExp rx;
    rx.setPattern("^(.*)/.*$");
    if (rx.indexIn(settings->fileName()) < 0) {
        notify("Ошибка чтения настроек", 2);
        return 1;
    }

    setZplDir(settings->value("zpl_dir", rx.cap(1)).toString());
    showAddress(settings->value("gui/address", true).toBool());
    showNotifications(settings->value("gui/notifications", true).toBool());

    QFileInfo fi(settings->fileName());
    if (!fi.exists())
        return saveSettings();

    if ((settings->status()) != 0) {
        notify(QString("Произошла ошибка #%1 при чтении настроек из файла %2.").arg(settings->status()).arg(settings->fileName()), 2);
        return 2;
    }

    notify(QString("Настройки прочитаны из %1.").arg(settings->fileName()), 0);
    return 0;
}

int MainWindow::saveSettings()
{
    settings->setValue("address", address_edit->text());
    settings->setValue("copies", copies_edit->text());
    settings->setValue("zpl_dir", zpl_dir.absolutePath());
    settings->setValue("zpl_file", zpl_list->currentData().toString());
    settings->setValue("window/maximized", isMaximized());
    if (!isMaximized()) {
        settings->setValue("window/width", width());
        settings->setValue("window/height", height());
    }
    settings->setValue("gui/address", show_address_act->isChecked());
    settings->setValue("gui/notifications", show_notifications_act->isChecked());
    settings->sync();

    notify("Настройки сохранены", 0);
    return 0;
}

int MainWindow::print()
{
    int ret = 0;
    QString address = address_edit->text();
    uint copies = copies_edit->text().toUInt();

    if (copies < 1) {
        notify("Некорректное колличество копий", 2);
        return 1;
    }

    if (copies > 9) {
        print_dialog->setText(QString("Вы действительно хотите напечатать <b>%1</b> этикеток?").arg(copies));
        if (print_dialog->exec() != 0) return 2;
    }

    QString zpl(zpl_edit->toPlainText());
    zpl.replace("^XZ", "^PQ" + QString::number(copies) + "^XZ");

    ret = printer->print(zpl, address);
    if (ret != 0)
        return 3;

    print_button->setDisabled(true);

    return 0;
}

int MainWindow::notify(QString text, int level)
{
    QString html = "[<span style='color:#888'>" + QDateTime::currentDateTime().toString("hh:mm:ss") + "</span>] <span style='";

    switch (level) {
        case 0:  html += "color: #008"; break;
        case 1:  html += "color: #f80"; break;
        case 2:  html += "color: #f00"; break;
        default: break;
    }

    html += "'>" + text + "</span><br>";

    notifications->moveCursor(QTextCursor::Start);
    notifications->insertHtml(html);

    return 0;
}


int MainWindow::processZplListCurrentIndexChanged(int i)
{
    if (i == -1) {
        zpl_edit->clear();
        return 1;
    }

    QString path = zpl_dir.path() + "/" + zpl_list->itemData(i).toString();
    QFileInfo info = QFileInfo(path);

    if (info.isFile()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            notify(QString("Файл %1 не доступен.").arg(path), 1);
            return 2;
        }
        zpl_edit->setPlainText(QString(file.readAll()).trimmed());
        file.close();
        return 0;
    }

    notify(QString("Файл %1 не найден.").arg(path), 1);

    return 3;
}

int MainWindow::saveZpl()
{
    return saveZplAs(zpl_dir.path() + "/" + zpl_list->currentData().toString());
}

int MainWindow::showAddress(bool s)
{
    address_edit->setVisible(s);
    address_label->setVisible(s);
    show_address_act->setChecked(s);
    return 0;
}

int MainWindow::showNotifications(bool s)
{
    notifications->setVisible(s);
    show_notifications_act->setChecked(s);
    return 0;
}

int MainWindow::setZplDir(QDir d)
{
    zpl_dir = d;
    zpl_dir_dialog->setDirectory(d);
    save_zpl_as_dialog->setDirectory(d);

    if (!zpl_dir.exists()) {
        notify(QString("Директория '%1' не существует.").arg(zpl_dir.absolutePath()), 1);
        return 1;
    }

    QStringList filters;
    filters << "*.zpl";
    zpl_dir.setNameFilters(filters);

    zpl_list->clear();
    QString zpl_file;

    QObject::disconnect(zpl_list, SIGNAL(currentIndexChanged(int)), this, SLOT(processZplListCurrentIndexChanged(int)));
    foreach (zpl_file, zpl_dir.entryList()) {
        zpl_list->addItem(zpl_file, zpl_file);
    }
    int i = zpl_list->findData(settings->value("zpl_file", "").toString());
    zpl_list->setCurrentIndex(i);
    processZplListCurrentIndexChanged(i);
    QObject::connect(zpl_list, SIGNAL(currentIndexChanged(int)), this, SLOT(processZplListCurrentIndexChanged(int)), Qt::UniqueConnection);
    return 0;
}

int MainWindow::setZplDir(QString s)
{
    return setZplDir(QDir(s));
}

int MainWindow::saveZplAs(QString path)
{
    QFileInfo info = QFileInfo(path);

    if (info.isFile()) {
        save_zpl_dialog->setText(QString("Вы действительно хотите перезаписать файл<br><br>%1?<br><br>Это действие невозможно отменить.").arg(path));
        if (save_zpl_dialog->exec() != 0) return 3;
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        notify(QString("Сохранение невозможно: файл '%1' не открывается.").arg(path), 1);
        return 1;
    }

    file.write(zpl_edit->toPlainText().toUtf8());
    file.close();

    setZplDir(info.absoluteDir());
    zpl_list->setCurrentIndex(zpl_list->findData(info.fileName()));


    notify(QString("Файл \"%1\" сохранен.").arg(path), 0);
    return 0;
}
