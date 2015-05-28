#include "mywidget.h"

#include <ctime>
#include <QtDebug>
#include <QPainter>
#include <qdrawutil.h>
#include <QMouseEvent>
#include <QMessageBox>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent),
      _L(20),
      _N(10),
      _NBombs(10)
{
    _visible.resize(_N);
    _neighbours.resize(_N);
    for (int i = 0; i < _N; i++)
    {
       _visible[i].resize(_N);
       _neighbours[i].resize(_N);
    }
    qsrand(time(0));
    for (int i = 0; i < _NBombs; i++)
    {
        bool ok = false;
        while(!ok)
        {
            int x = qrand() % _N;
            int y = qrand() % _N;
            ok = _neighbours[x][y] != eBomb;
            if (ok)
            {
                _neighbours[x][y] = eBomb;
                qDebug() << i << x << y;
            }
        }
    }

    for (int i = 0; i < _N; i++)
    {
        for (int j = 0; j < _N; j++)
        {
            if (_neighbours[i][j] == eBomb)
                continue;
            for (int n = -1; n <= 1; n++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    int ii = i + n;
                    int jj = j + m;
                    if (ii < 0 or jj < 0 or ii >= _N or jj >= _N)
                        continue;
                    if (_neighbours[ii][jj] == eBomb)
                        _neighbours[i][j]++;

                }
            }
        }
    }

}

MyWidget::~MyWidget()
{

}

void MyWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);


    for (int i = 0; i < _N; i++)
    {
        for (int j = 0; j < _N; j++)
        {
            QString s("0");

            if (_neighbours[i][j] == eBomb)
                s[0] = 'B';
            else
                s[0] = char('0' + _neighbours[i][j]);

            qDrawShadePanel(&painter, i*_L, j*_L, _L, _L, palette(),_visible[i][j], 2);
            if (_visible[i][j]  && _neighbours[i][j] > 0)
                painter.drawText(i*_L, j*_L, _L, _L, Qt::AlignCenter, s);
          //  painter.drawRect(i*_L, j*_L, _L, _L);
        }
    }
}

void MyWidget::mousePressEvent(QMouseEvent * ev)
{
    Qt::MouseButton button = ev->button();
    bool left_button = button == Qt::LeftButton;
    bool right_button = button == Qt::RightButton;
    if (!left_button && !right_button)
        return;

    QPoint point = ev->pos();
    int i = point.x()/_L;
    int j = point.y()/_L;
    qDebug() << point << i << j;
    if (i < _N && j < _N && i >= 0 && j >= 0)
    {
        if (_visible[i][j])
            return;
        _visible[i][j] = true;
    }
    else
        return;



    if ((_neighbours[i][j] == eBomb && left_button) or (right_button && _neighbours[i][j] != eBomb))
    {
        for (int i = 0; i < _N; i++)
        {
            for (int j = 0; j < _N; j++)
                _visible[i][j] = true;
        }
        // koniec pieśni
        QString s = left_button ? "Bomba!" : "Tu nie ma bomby";
        QMessageBox::information(this, "koniec gry", s, QMessageBox::Ok);
        this->setStatusTip(s);
    }
    if (_neighbours[i][j] == 0)
    {
        QVector <QPair<int, int> > v;
        v.push_back(QPair<int, int>(i, j));
        while(!v.empty())
        {
            QPair<int, int> para = v.back();
            int i = para.first;
            int j = para.second;
            v.pop_back();
            _visible[i][j] = true;
            if (_neighbours[i][j] != 0)
                continue;
            for (int n = -1; n <= 1; n++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    int ii = i + n;
                    int jj = j + m;
                    if (ii < 0 or jj < 0 or ii >= _N or jj >= _N)
                        continue;
                    if (!_visible[ii][jj])
                    {
                        _visible[ii][jj] = true;
                        v.push_back(QPair<int, int>(ii, jj));
                    }
                }
            }
        }
    }

    repaint();

}

void MyWidget::mouseReleaseEvent(QMouseEvent *)
{

}

void MyWidget::showEvent(QShowEvent *)
{

}



void MyWidget::resizeEvent(QResizeEvent *)
{
    QRect r = rect();
    _L = qMin(r.height()/_N, r.width()/_N);

}
