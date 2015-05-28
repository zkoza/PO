#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

class MyWidget : public QWidget
{
    Q_OBJECT

    enum {eBomb = 10};

    int _L;
    int _N;
    int _NBombs;
    QVector<QVector<bool> > _visible;
    QVector<QVector<int> > _neighbours;

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

signals:

public slots:

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void showEvent(QShowEvent *);
    virtual void resizeEvent(QResizeEvent *);
};

#endif // MYWIDGET_H
