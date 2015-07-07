
/**
 * @file
 * README file for Quiz Likert Scale.
 */

Quiz Likert Scale
Allows quiz creators to make a likert scale question type.

Sponsored by: Focal55 Inc, Sherman Luiggi
Code: focal55


CONTENTS
--------

1.  Introduction
2.  Installation
3.  Configuration

----
1. Introduction
This module is an attempt to allow quiz creators to use a Likert style questions using the quiz module.

The likert_scale module lets the user create single answer questions and multiple answer questions.
Advanced feedback and scoring options are also available.

The likert scale module is based on the OO framework of the quiz project and built off of the likert scalale module.

Likert Definition: https://en.wikipedia.org/wiki/Likert_scale

----
2. Installation

To install, unpack the module to your modules folder, and simply enable the module at Admin > Build > Modules.

Several database tables prefixed with quiz_likert_scale are installed to have a separate storage for this module.

----
3.  Configuration
Settings are to be found here: admin/quiz/questions_settings

The only available setting is the number of default alternatives. This decides how many alternatives
will be shown on the node-form by default. The question creator can add more by pushing the
add more alternatives button, and he can always leave some alternatives blank if not all the shown
alternatives are needed.