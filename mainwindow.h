#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>
#include <QMediaPlayer>
#include "timerwdg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct Player
    {
       QString name;
       int score;//кол-во ответов
       int nQuest;//кол-во вопросов
    };
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onNewGame();
    void onExit();
    void onAnswer();
    void onPlay();
    void onStop();
    void onNewUser();
    void onAddUser(QString);
    void onChooseUser();
protected:
    void closeEvent(QCloseEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);
    void paintEvent(QPaintEvent *event);



private:
    QBrush brush;
    void readQuestions();
    void readUsers();
    void saveUsers();
    void testGame();
    void showGame(bool visible);
    int m_rightAnswer;
    void onNext();
    QAction* newGame;
    QMenu* file;
    QMenu* users;
    Ui::MainWindow *ui;
    TimerWdg timerWdg;
    enum eTypeQuest
    {
        eTypeQuest_Usual,
        eTypeQuest_Graphic,
        eTypeQuest_Music,
    };
    QString m_MusicFile;
    QMediaPlayer m_Player;
    int numAnswers = 0;
    int numQuestions = 0;//сколько в текущей игре вопросов
    struct Question
    {
        eTypeQuest type;
        QString quest;
        QString answers[4];
        int righAns;
    };
    Player* m_Gamer;

    QList<Player> m_Users;
    QQueue<Question> m_Questions;//хранится список вопросов
};

#endif // MAINWINDOW_H
