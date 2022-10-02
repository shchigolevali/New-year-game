#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QToolButton"
#include "QFile"
#include "QPicture"
#include "QStackedWidget"
#include "timerwdg.h"
#include "dialogname.h"
#include "QMouseEvent"
#include<QFontDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    users = new QMenu("Users");
    readUsers();
    for(int i =0; i < m_Users.size(); ++i)
    {
        QAction* user = new QAction(m_Users.at(i).name);//для каждого эл.в списке создаем QAction
        user->setProperty("number",i);// в списке m_Users
        users->addAction(user);
        connect(user, SIGNAL(triggered(bool)), this, SLOT(onChooseUser()));//*после выпонения слота мы выбираем пользователя
    }
    file = new QMenu("General");
    newGame = new QAction("Start new game");
    newGame->setShortcut(QKeySequence(Qt::CTRL+ Qt::Key_N));
    newGame->setEnabled(false);//*недоступно пока не выберем
    QAction* exit = new QAction("Exit");
    exit->setShortcut(QKeySequence(Qt::CTRL+ Qt::Key_Q));
    QAction* newUser = new QAction("New User");
    newUser->setShortcut(QKeySequence(Qt::CTRL+ Qt::Key_U));
    file->addAction(newGame);
    file->addAction(newUser);
    file->addMenu(users);
    file->addAction(exit);
    ui->menuBar->addMenu(file);
    installEventFilter(this);

    connect(newGame,SIGNAL(triggered(bool)),this, SLOT(onNewGame()));
    connect(newUser, SIGNAL(triggered(bool)), this, SLOT(onNewUser()));
    connect(exit,SIGNAL(triggered(bool)),this, SLOT(onExit()));

    QAction* answer = new QAction("Answer");
    answer->setShortcut(QKeySequence(Qt::Key_1));
    ui->btn1->setDefaultAction(answer);

    connect(ui->btn1, SIGNAL(triggered(QAction*)), this, SLOT(onAnswer()));


    connect(ui->btn2, SIGNAL(triggered(QAction*)), this, SLOT(onAnswer()));
    answer = new QAction("Answer");
    answer->setShortcut(QKeySequence(Qt::Key_2));
    ui->btn2->setDefaultAction(answer);


    connect(ui->btn3, SIGNAL(triggered(QAction*)), this, SLOT(onAnswer()));
    answer = new QAction("Answer");
    answer->setShortcut(QKeySequence(Qt::Key_3));
    ui->btn3->setDefaultAction(answer);


    connect(ui->btn4, SIGNAL(triggered(QAction*)), this, SLOT(onAnswer()));
    answer = new QAction("Answer");
    answer->setShortcut(QKeySequence(Qt::Key_4));
    ui->btn4->setDefaultAction(answer);
    connect(ui->playBtn, SIGNAL(pressed()), this, SLOT(onPlay()));
    showGame(false);
    timerWdg.hide();
    ui->timerLayout->addWidget(&timerWdg);

    connect(&timerWdg, SIGNAL(timeout()), this ,SLOT(onAnswer()));
    setMouseTracking(true);
    centralWidget()-> setStyleSheet("background-color: #FFDEAD");
    ui->qstWdg->setFont(QFontDialog::getFont(0, QApplication::font()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewGame()
{
    qDebug()<< "onNewGame";
    readQuestions();//*считываем вопросы

    m_Gamer->nQuest += m_Questions.size();//*сохранение общего списка вопросов
    numQuestions = m_Questions.size();
    onNext();//*отображение
    showGame(true);
}

void MainWindow::onExit()
{
    qDebug()<< "onExit";
    saveUsers();
    exit(0);
}

void MainWindow::onAnswer()
{
    timerWdg.stop();
    timerWdg.hide();
    if(!m_MusicFile.isEmpty())
    {
        onStop();
        m_MusicFile.clear();
    }
    timerWdg.stop();
    timerWdg.hide();
    qDebug() << "onAnswer" << sender()->property("answer").toInt();
    if(sender()->property("answer").toInt() == m_rightAnswer)//*если совпадает с правильным ответом,то увеличиваем счет
    {
        //TODO:
        qDebug()<<"right";
        ++m_Gamer->score;
        ++numAnswers;
    }
    else
    {
        qDebug()<<"fail";
    }
    if(!m_Questions.isEmpty())
    {
        onNext();
    }
    else
    {

        ui->qstWdg->setCurrentIndex(3);

        ui->endLbl->setText(QString("Вы ответили на %1 вопросов из %2\n"
                                    "Ваш общий результат %3 из %4").arg(numAnswers).arg(numQuestions).arg(m_Gamer->score).arg(m_Gamer->nQuest));
        showGame(false);
    }



}

void MainWindow::onPlay()
{
    m_Player.setMedia(QUrl(m_MusicFile));
    m_Player.setVolume(50);
    m_Player.play();
}

void MainWindow::onStop()
{
    m_Player.stop();
}

void MainWindow::onNewUser()
{
    DialogName nameDlg;//*создаем диалоговое окно
    connect(&nameDlg, SIGNAL(userName(QString)), this, SLOT(onAddUser(QString)));//передается имя
    nameDlg.exec();
}

void MainWindow::onAddUser(QString name)
{
    for(int i = 0; i < m_Users.size(); ++i)//есть ли пользователь с таким именем
    {
        if(m_Users.at(i).name == name)
            return;
    }
    Player user;//добавляем новое имя
    user.name= name;
    user.score = 0;//*объявляем все его счета
    user.nQuest = 0;
    m_Users.append(user);//добавление в список
    QAction* act = new QAction(user.name);//*создаем новый QAction
    act->setProperty("number",m_Users.size()-1);
    connect(act,SIGNAL(triggered(bool)), this, SLOT(onChooseUser()));//*сигнал на выбор пользователя
    users->addAction(act);
}

void MainWindow::onChooseUser()//выбор пользователя
{
    m_Gamer = &(m_Users[sender()->property("number").toInt()]);//*sender(тот,кто посылает сигнал(куэкшен)
    newGame->setEnabled(true);//*пока не выбрали пользователя не начинаем игру
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    saveUsers();
    QWidget::closeEvent(evt);//сохранение данных пользователя
}

void MainWindow::mouseMoveEvent(QMouseEvent *evt)
{

    ui->mouseCoord->show();
    ui->mouseCoord->setText(QString("Mouse coordinates: X= %1, Y=%2").arg(evt->x()).arg(evt->y()));
}



void MainWindow::readQuestions()
{
    QFile f(":/questions.txt");
    f.open(QFile::ReadOnly);//*открываем файл на чтение
    if(f.isOpen())
    {
        while(!f.atEnd())
        {

            Question quest;
            quest.type = (eTypeQuest) f.readLine().toInt();
            quest.quest = f.readLine();
            for(int i = 0; i <4; ++i)//*с ответами
            {
                quest.answers[i] = f.readLine();
            }
            quest.righAns = f.readLine().toInt();
            m_Questions.enqueue(quest);//*добавляем в очередь
        }
    }
}

void MainWindow::readUsers()
{
    QFile f("users.txt");
    f.open(QFile::ReadOnly);
    if(f.isOpen())
    {
        while(!f.atEnd())//* заходим в файл и считываем построчно в поля нашей структуры
        {
            Player user;
            user.name = f.readLine();
            user.name.remove('\n');
            user.score = f.readLine().toInt();
            user.nQuest = f.readLine().toInt();
            m_Users.append(user);//добавляет игрока
        }
    }
}

void MainWindow::saveUsers()
{
    QFile f("users.txt");
    f.open(QFile::Truncate| QFile::WriteOnly);//*все удалено/файл будет создан
    if(f.isOpen())
    {
        for(int i =0; i < m_Users.size(); ++i)
        {
            f.write(QByteArray(m_Users.at(i).name.toStdString().c_str(),m_Users.at(i).name.size()));
            f.write("\n");
            QString score = QString::number(m_Users.at(i).score);
            f.write(QByteArray(score.toStdString().c_str(),score.size()));
            f.write("\n");
            QString nQuest = QString::number(m_Users.at(i).nQuest);
            f.write(QByteArray(nQuest.toStdString().c_str(),nQuest.size()));
            f.write("\n");
        }
    }
}

void MainWindow::testGame()
{
    Question tmp;
    tmp.quest = "qwe";
    tmp.answers[0] = "answer1";
    tmp.answers[1] = "answer2";
    tmp.answers[2] = "answer3";
    tmp.answers[3] = "answer4";
    tmp.righAns = 2;
    m_Questions.append(tmp);
}

void MainWindow::showGame(bool visible)//*показываем кнопки
{

    ui->btn1->setVisible(visible);
    ui->btn2->setVisible(visible);
    ui->btn3->setVisible(visible);
    ui->btn4->setVisible(visible);
}
void MainWindow::paintEvent(QPaintEvent *evt)
{

//    QPainter paint(this);
//    paint.begin(this);



//   paint.setBrush(QBrush(QColor(11,11,11),Qt::SolidPattern));
//   paint.drawRect(0,0,width(),height());

//   QRect r(0, 0, width(), height());
//   paint. setFont (QFont ( "Times", 25, QFont:: DemiBold ));
//   paint.drawRect(r);
//   paint.drawТext(r, Qt::AlignCenter 1 Qt::TextWordWrap, "Draw Text");
}

void MainWindow::onNext()//отображение вопросов
{
    Question tmp = m_Questions.dequeue();
    timerWdg.show();
    timerWdg.start();
    ui->btn1->setText(tmp.answers[0]);
    ui->btn2->setText(tmp.answers[1]);
    ui->btn3->setText(tmp.answers[2]);
    ui->btn4->setText(tmp.answers[3]);

    switch (tmp.type)//*в зависимости от типа вопроса
    {
    case eTypeQuest_Usual:
    {
        ui->txtQst->setText(tmp.quest);
        ui->qstWdg->setCurrentIndex(0);
        break;
    }
    case eTypeQuest_Graphic:
    {
        tmp.quest.remove('\n');
        QStringList list = tmp.quest.split(';');//разднляем строку на несколько
        ui->imgQst->setText(list[0]);
        QPixmap mp = QPixmap(list[1]);

        mp = mp.scaled(ui->imgLbl->width(), ui->imgLbl->height());//*преобразование к размерам
        ui->imgLbl->setPixmap(mp);
        ui->imgLbl->setVisible(true);
        ui->qstWdg->setCurrentIndex(1);
        break;
    }
    case eTypeQuest_Music:
    {
        tmp.quest.remove('\n');
        QStringList list = tmp.quest.split(';');
        ui->musicQst->setText(list[0]);
        m_MusicFile = list[1];
        ui->qstWdg->setCurrentIndex(2);
        break;
    }
    }

    m_rightAnswer = tmp.righAns;//сохранение правильного ответа
}
