#include "animationfilter.h"

/*!
 * \brief An accessor for the movie variable.
 * \return The movie stored inside the filter
 */
QMovie* AnimationFilter::getMovie() {
    return movie;
}

/*!
 * \brief A mutator for the movie variable.
 * \param mov The new movie uploaded by the user
 */
void AnimationFilter::setMovie(QMovie *mov) {
    movie = mov;
}
