#ifndef ANIMATIONFILTER_H
#define ANIMATIONFILTER_H

#include <QMovie>
#include "basefilter.h"

/*!
 * \brief An abstract base class for filters dealing with GIF animated images.
 */
class AnimationFilter: public BaseFilter {
public:
    QMovie* getMovie();
    void setMovie(QMovie*);
protected:
    QMovie *movie; /*!< A gif animated image stored inside the filter */
};

#endif // ANIMATIONFILTER_H
