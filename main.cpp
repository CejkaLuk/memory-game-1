#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName("Memory Game");
    QApplication::setApplicationVersion("1.0");
    QApplication::setWindowIcon(QIcon(":icons/taskBarIcon.png"));


    MainWindow w;
    w.show();

    return a.exec();
}

/******** TODO
 ** Bugs:
 *      - [ FIXED ] When surrender is declined, it still surrenders
 *      - [ WORKAROUND ] Resizing window, doesn't expand the board etc. ///http://doc.qt.io/qt-5/qtwidgets-widgets-scribble-example.html
 *      - [ PC dependent ] When save or load is called, a message is printed out in console: "Gtk-Message: GtkDialog mapped without a transient parent. This is discouraged."
 *              -- This happens only on school computer?
 *      - [ FIXED ] When the game is first launched or when a file is loaded, the cards dont fit into the graphicsView frame on the left side.
 *              -- The height of the cards is smaller, but their width is larger.
 *
 ** When two cards are revealed and we click somewhere else, set them both to back and reveal what we want to click on.
 *
 ** [ DONE ] Implement two player
 *      - Fix not changing of players
 *      - Fix incorrect number of matched cards assigned
 *
 ** Save and Load:
 *          - [ FIXED ] If the player saves/loads a saved game, the elapsed steps will not be present immediately after loading, but only after the player has played one new step.
 *                      -- This causes New Game to not ask the player "are you sure you want to abandon...", which is a bug.
 *
 ** Animation:
 *          - [ DONE ] When clicking cards, I want them to "press into" the board. So move them diagonally to the bottom right a little bit and then back upon mouse release.
 *          - [ DONE ] To reveal a card, have a spinning animation.
*/
