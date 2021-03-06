 #ifndef GAMSCENE_H
#define GAMSCENE_H

#include <QQmlListProperty>
#include <vector>
#include "block.h"

class GameScene: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Block> blockArray READ blockArray NOTIFY blockChanged)
    Q_PROPERTY(QList<int> remind READ remind)
    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged)
    Q_PROPERTY(QList<int> passScore READ passScores NOTIFY passScoreChanged)

    Q_CLASSINFO("DefaultProperty", "block")
public:
    GameScene(QObject *parent = 0):QObject(parent){}
    GameScene(int i);
    Q_INVOKABLE void initScene(GameScene *x);

    QQmlListProperty<Block> blockArray();

    Q_INVOKABLE void refresh(int levelNumber);

    Q_INVOKABLE Block *block(int number) const;
    Q_INVOKABLE void swap(int start_x,int start_y,int end_x, int end_y);
    Q_INVOKABLE void control(int x_one, int y_one, int x_two, int y_two);
    int sameOfNumber(QList<int> &block, int x, int y, int type,int x_Or_y);
    void clearBlocks(QList<int> &block);
    Q_INVOKABLE void moveBlocks();


    Q_INVOKABLE void setScore(int score);

    Q_INVOKABLE int score() const;

    int passScore(int i) const;
    Q_INVOKABLE void setPassScore(int x, int level);
    void initPassScore();

    Q_INVOKABLE void readScoreIn();
    Q_INVOKABLE int passNumber(){
        return m_passScore.size();
    }

    void setTarget();
    Q_INVOKABLE int target(int i) const;

    Q_INVOKABLE void setBlocks(const QList<Block *> &blocks);
    Q_INVOKABLE QList<Block *> blocks() const;
    Q_INVOKABLE void changedType(int index,int type);

    Q_INVOKABLE int number(int i) const;
    void setNumber(int levelnumber);
    void numberChanged(int type,int number);

    QList<int> passScores() const;

    Q_INVOKABLE void clearBlock(int x,int y);
    Q_INVOKABLE void clearLineX(int x);
    Q_INVOKABLE void clearLineY(int y);
    Q_INVOKABLE void clearType(int type);

    void setRemind();
    Q_INVOKABLE int reminds(int i);
    bool swapOrNot(int start_x, int start_y, int end_x, int end_y);


    QList<int> remind() const;

signals:
    void blockChanged();
    void scoreChanged();
    void passScoreChanged();
    void typeChanged(int begin_x,int begin_y,int end_x,int end_y);
    void typeChanged_down(int begin_x,int begin_y,int end_x,int end_y);
    void typeDestroy(int x,int y);
    void typeNew(int x,int y);
    void clearAllBlocks();
    void fallDownAllBlock();
    void cannotClear();
    void readScoreChanged();

private:
    QList<int> m_passScore;
    int m_score = 0;
    QList<int> m_target;
    QList<int> m_number;
    QList<Block *> m_blocks;
    QList<int> m_remind;
    static void appendBlock(QQmlListProperty<Block> *list, Block *msg);
    static int countBlock(QQmlListProperty<Block> *list);
    static Block *atBlock(QQmlListProperty<Block> *list, int i);
    static void clearBlock(QQmlListProperty<Block> *list);

};

#endif // GAMSCENE_H
