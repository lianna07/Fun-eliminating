#include "gamescene.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

QQmlListProperty<Block> GameScene::blockArray()
{
    return QQmlListProperty<Block>(this,nullptr,GameScene::appendBlock,GameScene::countBlock,GameScene::atBlock,GameScene::clearBlock);
}

void GameScene::appendBlock(QQmlListProperty<Block> *list, Block *msg)
{
    GameScene *game = qobject_cast<GameScene *>(list->object);
    if(msg) {
        msg->setParent(game);
        game->m_blocks.append(msg);
    }
    emit game->blockChanged();
}

int GameScene::countBlock(QQmlListProperty<Block> *list)
{
    GameScene *g = qobject_cast<GameScene *>(list->object);
    if(g)
        return g->m_blocks.count();
    return 0;
}

Block *GameScene::atBlock(QQmlListProperty<Block> *list, int i)
{
    GameScene *g = qobject_cast<GameScene *>(list->object);
    if(g)
        return g->m_blocks.at(i);
    return nullptr;
}

void GameScene::clearBlock(QQmlListProperty<Block>  *list)
{
    GameScene *g = qobject_cast<GameScene *>(list->object);
    if(g)
    {
        g->m_blocks.clear();
        emit g->blockChanged();
    }
}

void GameScene::refresh(int levelNumber)
{
    srand((unsigned)time(NULL));
    for(int i = 0;i != 12;i++) {
        for(int y = 0;y != 8;y++) {
            int type = rand() % BLOCK_TYPE;
            m_blocks[i * 8 + y]->setType(type);
        }
    }
    setTarget();
    setNumber(levelNumber);
    for(int i = 0;i != m_target.size();i++)
    {
        std::cout << "fresh" << m_target[i] << "   " << m_number[i] << std::endl;
    }
}


Block *GameScene::block(int number) const
{
    return m_blocks[number];
}

GameScene::GameScene(int i)
{
    srand((unsigned)time(NULL));
    for(int x = 0;x != 12;x++) {
        for(int y = 0;y != 8;y++) {
            int type = rand() % i;
            Block *b = new Block(x,y,type);
            m_blocks.push_back(b);
        }
        std::cout << std::endl;
    }

}


void GameScene::initScene(GameScene *x)
{
    this->m_blocks.clear();
    this->m_blocks = x->blocks();

    this->m_score = x->score();

    this->m_target.clear();
    this->m_target = x->m_target;
    this->m_number.clear();
    this->m_number = x->m_number;
//    this->m_passScore = x->m_passScore;
}

void GameScene::swap(int start_x, int start_y, int end_x, int end_y)
{

    int type_1 = m_blocks[start_x * 8 + start_y]->type();
    int type_2 = m_blocks[end_x * 8 + end_y]->type();
    m_blocks[start_x * 8 + start_y]->setType(type_2);
    m_blocks[end_x * 8 + end_y]->setType(type_1);

    QList<int> block;
    for(int i = 0;i != 96;i++) {
        block.push_back(m_blocks[i]->type());
    }
    QList<int> b = block;
    int number_x1 = sameOfNumber(b,start_x,start_y,type_2,0);
    b = block;
    int number_y1 = sameOfNumber(b,start_x,start_y,type_2,1);
    b = block;
    int number_x2 = sameOfNumber(b,end_x,end_y,type_1,0);
    b = block;
    int number_y2 = sameOfNumber(b,end_x,end_y,type_1,1);
    if(number_x1 < 3 && number_x2 < 3 && number_y1 < 3 && number_y2 < 3) {
        m_blocks[start_x * 8 + start_y]->setType(type_1);
        m_blocks[end_x * 8 + end_y]->setType(type_2);
    }
    else {
        emit typeChanged(start_x,start_y,end_x,end_y);
    }
}


void GameScene::control(int x_one, int y_one,int x_two,int y_two)
{
    bool clearNumber = true;
        QList<int> block;
        for(int i = 0;i != 96;i++) {
            block.push_back(m_blocks[i]->type());
        }
        QList<int> b[2];
        b[0] = b[1] = block;
        for(int i = 0;i != 2;i++) {
            int type;
            int number_x,number_y;
            if(i == 0)  {
                type = block[x_one * 8 + y_one];
                number_x = sameOfNumber(b[0],x_one,y_one, type,0);
                number_y = sameOfNumber(b[1],x_one,y_one,type,1);
                std::cout << "_______" << x_one << y_one << number_x << number_y << std::endl;
            }
            else {
                std::cout << x_two << y_two<< number_x << number_y << std::endl;
                type = block[x_two * 8 + y_two];
                number_x = sameOfNumber(b[0],x_two,y_two, type,0);
                number_y = sameOfNumber(b[1],x_two,y_two,type,1);
            }
            if(number_x >=3 && number_y >= 3) {
                clearNumber = false;
                numberChanged(type,number_x + number_y - 1);
                clearBlocks(b[0]);
                clearBlocks(b[1]);
                setScore(number_x + number_y - 1);
                std::cout << "clear all ---" << number_x << number_y << type <<std::endl;
            }
            else if(number_x >= 3 && number_y < 3) {
                numberChanged(type,number_x);
                clearNumber = false;
                clearBlocks(b[0]);
                setScore(number_x);
                std::cout << "clear x ---" << number_x << number_y <<std::endl;
            }
            else if(number_y >= 3 && number_x < 3) {
                numberChanged(type,number_y);
                clearNumber = false;
                clearBlocks(b[1]);
                setScore(number_y);
                std::cout << "clear y --- " << number_y << number_x<<std::endl;
            }
            block.clear();
            for(int i = 0;i != 96;i++) {
                block.push_back(m_blocks[i]->type());
            }
            b[0] = b[1] = block;

        }
        for(int x =0;x != 12;x++) {
            for(int y = 0;y != 8;y++) {
                int type = block[x * 8 + y];
                int number_x = sameOfNumber(b[0],x,y, type,0);
                int number_y = sameOfNumber(b[1],x,y,type,1);

                if(number_x >=3 && number_y >= 3) {
                    clearNumber = false;
                    numberChanged(type,number_x + number_y - 1);
                    clearBlocks(b[0]);
                    clearBlocks(b[1]);
                    setScore(number_x + number_y - 1);
                    std::cout << "clear all ---" << number_x << number_y << type <<std::endl;
                }
                else if(number_x >= 3 && number_y < 3) {
                    numberChanged(type,number_x);
                    clearNumber = false;
                    clearBlocks(b[0]);
                    setScore(number_x);
                    std::cout << "clear x ---" << number_x << number_y <<std::endl;
                }
                else if(number_y >= 3 && number_x < 3) {
                    numberChanged(type,number_y);
                    clearNumber = false;
                    clearBlocks(b[1]);
                    setScore(number_y);
                    std::cout << "clear y --- " << number_y << number_x<<std::endl;
                }
                block.clear();
                for(int i = 0;i != 96;i++) {
                    block.push_back(m_blocks[i]->type());
                }
                b[0] = b[1] = block;
            }
        }
        if(clearNumber) {
            emit cannotClear();

        }
        else {
            emit clearAllBlocks();
        }
}

int GameScene::sameOfNumber(QList<int> &block,int x,int y,int type,int x_Or_y)
{
    if(x >= 12 || x < 0 || y >= 8 || y < 0)
        return 0;
    int b = block[x * 8 + y];
    if(b == -1)
        return 0;
    if(b != type)
        return 0;
    block[x * 8 + y] = -1;
    int count = 1;
    if(x_Or_y == 0) {
        count += sameOfNumber(block,x + 1,y,type,0);
        count += sameOfNumber(block,x - 1,y,type,0);
    }
    else if(x_Or_y == 1){
        count += sameOfNumber(block,x,y + 1,type,1);
        count += sameOfNumber(block,x,y - 1,type,1);
    }

    return count;
}

void GameScene::clearBlocks(QList<int> &block)
{
    for(int i = 0; i < 96; i++) {
        if(block[i] == -1) {
            auto blocks = m_blocks[i];
            if(blocks->type() != -1) {
                m_blocks[i]->setType(-1);
                std::cout << "clear  " << i / 8 << "  " << i % 8 << std::endl;
            }
        }
    }
}

void GameScene::moveBlocks()
{
    srand((unsigned)time(NULL));
    for(int y = 0;y < 8; y++){
        int nullBlockNumber = 0;
        for(int x = 11; x >= 0;x--){
            if(m_blocks[x * 8 + y]->type() == -1){
                nullBlockNumber++;
            }
            else if(nullBlockNumber != 0 && m_blocks[x * 8 + y]->type() !=  -1) {
                for(int move = 1;move != nullBlockNumber + 1;move++) {
                    m_blocks[(x + move) * 8 + y]->setType(m_blocks[(x + move - 1) * 8 + y]->type());
                    m_blocks[(x + move - 1) * 8 + y]->setType(-2);
                    emit typeChanged_down(x + move,y,x + move - 1,y);
                }
            }
        }
        for(int x = 0;x != nullBlockNumber;x++) {
            QList<int> r;
            for(int i = 0;i != 100; i++) {
                int t = rand() % 5;
                r.push_back(t);
            }
            m_blocks[x * 8 + y]->setType(r[nullBlockNumber - x - 1]);
            emit typeNew(x,y);
        }

    }
    setRemind();
    emit fallDownAllBlock();
}

void GameScene::setScore(int score)
{
    m_score += score;
}


int GameScene::score() const
{
    return m_score;
}

int GameScene::passScore(int i) const
{
    return m_passScore[i];
}

void GameScene::setPassScore(int x,int level)
{
    if(level > m_passScore.size())
        m_passScore.push_back(x);
    else if(m_passScore[level - 1] < x){
        m_passScore[level - 1] = x;
    }
}

void GameScene::initPassScore()
{

    std::string line;
    std::ifstream ifs("../Fun-eliminating/assets/pass");

    while (getline(ifs,line)){
        std::istringstream record(line);
        int number;
        record >> number;
        if(number != 0)
            m_passScore.push_back(number);
    }

}


void GameScene::readScoreIn()
{
    std::ofstream ofs("../Fun-eliminating/assets/pass");
    if (ofs){
//        int sth = m_score;
//        if (m_score < 1000) sth = 1;
//        else if (1000 <= m_score && m_score < 2000) sth = 2;
//        else if (m_score <= 2000) sth = 3;
//        else {
//            sth = -1;
//        }
//        if(sth != -1) {
//            ofs << sth <<std::endl;
//            m_passScore.push_back(sth);
//        }
        for(int i = 0;i != m_passScore.size();i++) {
            ofs << m_passScore[i] << std::endl;
        }
    }

    ofs.close();
}

void GameScene::setTarget()
{
    m_target.clear();
    srand((unsigned)time(NULL));
    int arr[BLOCK_TYPE];
    for(int i = 0;i != BLOCK_TYPE;i++) {
        arr[i] = i;
    }
    std::random_shuffle(arr,arr+BLOCK_TYPE-1);
    for(int i = 0;i != 3;i++) {
        m_target.push_back(arr[i]);
    }

}

int GameScene::target(int i) const
{
    return m_target[i];
}


void GameScene::setBlocks(const QList<Block *> &blocks)
{
    m_blocks = blocks;
}

QList<Block *> GameScene::blocks() const
{
    return m_blocks;
}

void GameScene::changedType(int index, int type)
{
    m_blocks[index]->setType(type);

}

int GameScene::number(int i) const
{
    return m_number[i];
}

void GameScene::setNumber(int levelnumber)
{
    m_number.clear();
    srand((unsigned)time(NULL));
    int a;
    for(int i = 0;i != 3;i++) {
        a = levelnumber * 15;
        m_number.push_back(a);
    }
}

void GameScene::numberChanged(int type,int number)
{
    for (int i  = 0;i != m_target.size();i++) {
        if(type == m_target[i] && m_number[i] > 0){
            m_number[i] -= number;
        }

    }
}

QList<int> GameScene::passScores() const
{
    return m_passScore;
}

void GameScene::clearBlock(int x, int y)
{
    int type  = m_blocks[x * 8 + y]->type();
    m_blocks[x * 8 + y]->setType(-1);
    numberChanged(type,1);
    emit clearAllBlocks();
}

void GameScene::clearLineX(int x)
{
    for(int i = 0;i != 8;i++) {
        int type  = m_blocks[x * 8 + i]->type();
        m_blocks[x * 8 + i]->setType(-1);
        numberChanged(type,1);
    }
    emit clearAllBlocks();
}

void GameScene::clearLineY(int y)
{
    for(int i = 0;i != 12;i++) {
        int type  = m_blocks[i * 8 + y]->type();
        m_blocks[i * 8 + y]->setType(-1);
        numberChanged(type,1);
    }
    emit clearAllBlocks();
}

void GameScene::clearType(int type)
{
    for(int x = 0;x != 12;x++) {
        for (int y = 0;y != 8;y++) {
            int t = m_blocks[x * 8 + y]->type();
            if(t == type) {
                m_blocks[x * 8 + y]->setType(-1);
                numberChanged(type,1);
            }
        }
    }
    emit clearAllBlocks();
}


int GameScene::reminds(int i)
{
    return m_remind[i];
}

void GameScene::setRemind()
{
    m_remind.clear();
    bool write = false;
    for(int x = 0; x != 12;x++) {
        for (int y = 0;y != 8;y++) {
                if(swapOrNot(x,y,x + 1,y) && !write) {
                    m_remind.push_back(x);
                    m_remind.push_back(y);
                    m_remind.push_back(x + 1);
                    m_remind.push_back(y);
                    write = true;
                }
                else if(swapOrNot(x,y,x,y + 1) && !write) {
                    m_remind.push_back(x);
                    m_remind.push_back(y);
                    m_remind.push_back(x);
                    m_remind.push_back(y + 1);
                    write = true;
                }
                else if(swapOrNot(x,y,x,y -1) && !write) {
                    m_remind.push_back(x);
                    m_remind.push_back(y);
                    m_remind.push_back(x);
                    m_remind.push_back(y -1);
                    write = true;
                }
                else if(swapOrNot(x,y,x - 1,y) && !write) {
                    m_remind.push_back(x);
                    m_remind.push_back(y);
                    m_remind.push_back(x - 1);
                    m_remind.push_back(y);
                    write = true;
                }
        }
    }
    std::cout <<"??????????????????" << m_remind[0] << m_remind[1] << m_remind[2] << m_remind[3] << std::endl;
}

bool GameScene::swapOrNot(int start_x, int start_y, int end_x, int end_y)
{
    if(start_x < 0 || start_x > 11 ||
            start_y < 0 || start_y > 11 ||
            end_x < 0 || end_x > 7 ||
            end_y < 0 || end_y > 7)
        return false;
    int type_1 = m_blocks[start_x * 8 + start_y]->type();
    int type_2 = m_blocks[end_x * 8 + end_y]->type();
    m_blocks[start_x * 8 + start_y]->setType(type_2);
    m_blocks[end_x * 8 + end_y]->setType(type_1);

    QList<int> block;
    for(int i = 0;i != 96;i++) {
        block.push_back(m_blocks[i]->type());
    }
    QList<int> b = block;
    int number_x1 = sameOfNumber(b,start_x,start_y,type_2,0);
    b = block;
    int number_y1 = sameOfNumber(b,start_x,start_y,type_2,1);
    b = block;
    int number_x2 = sameOfNumber(b,end_x,end_y,type_1,0);
    b = block;
    int number_y2 = sameOfNumber(b,end_x,end_y,type_1,1);
    if(number_x1 < 3 && number_x2 < 3 && number_y1 < 3 && number_y2 < 3) {
        m_blocks[start_x * 8 + start_y]->setType(type_1);
        m_blocks[end_x * 8 + end_y]->setType(type_2);
        return false;
    }
    else {
        m_blocks[start_x * 8 + start_y]->setType(type_1);
        m_blocks[end_x * 8 + end_y]->setType(type_2);
        return true;
    }
}

QList<int> GameScene::remind() const
{
    return m_remind;
}
