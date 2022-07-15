#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSettings>
#include <QTextEdit>
#include <QDateTime>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include "zplprinter.h"
#include "zplhighlighter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    int loadSettings();
    int saveSettings();
    int saveZpl();
    int print();
    int processZplListCurrentIndexChanged(int);
    int showAddress(bool);
    int showNotifications(bool);
    int setZplDir(QDir);
    int setZplDir(QString);
    int saveZplAs(QString);
signals:
    void quit();
public slots:
    int notify(QString text, int level = 0);
private:
    QWidget *central_widget;
    QGridLayout *layout;
    QComboBox *zpl_list;
    QTextEdit *zpl_edit;
    QTextEdit *notifications;
    QLabel *address_label;
    QLineEdit *address_edit;
    QLineEdit *copies_edit;
    QSettings *settings;
    ZPLPrinter *printer;
    QPushButton *print_button;
    QMessageBox *print_dialog;
    QMessageBox *save_zpl_dialog;
    QMessageBox *about_dialog;
    QMenu *file_menu;
    QMenu *view_menu;
    QMenu *misc_menu;

    QAction *print_act;
    QAction *save_zpl_act;
    QAction *save_zpl_as_act;
    QAction *select_zpl_dir_act;
    QAction *save_settings_act;
    QAction *show_address_act;
    QAction *show_notifications_act;
    QAction *about_act;
    QAction *quit_act;

    QFileDialog *zpl_dir_dialog;
    QFileDialog *save_zpl_as_dialog;

    QDir zpl_dir;
    ZPLHighlighter *highlighter;
};
#endif // MAINWINDOW_H
