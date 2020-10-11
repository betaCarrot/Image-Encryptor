#ifndef GIFFILTER_H
#define GIFFILTER_H

#include <QVector>
#include <QString>
#include <QLinkedList>
#include "animationfilter.h"
#include "gif.h"

/*!
 * \brief The filter for inserting a secret image into one frame of an animated GIF image.
 */
class InsertFilter: public AnimationFilter {
public:
    void setMovie(QMovie*);
    QString getDestination();
    void setDestination(QString);
    virtual void apply() override;
private:
    QLinkedList<QImage> list; /*!< A linked list that stores individual frames of the gif for fast insertion */
    QString destination; /*!< The destination folder of the result gif image to be saved */
};

#endif // GIFFILTER_H
