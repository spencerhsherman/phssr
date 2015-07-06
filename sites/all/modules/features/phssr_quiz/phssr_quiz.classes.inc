<?php
class PhssrScaleResponse extends ScaleResponse {
  /**
   * Implementation of score
   *
   * @see QuizQuestionResponse#score()
   */
  public function score() {
    return (bool) $this->getResponse();
  }
}