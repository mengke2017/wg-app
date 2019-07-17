#include <QtXml>
#include <QFile>
#include <QIODevice>
#include <QDomNodeList>
#include "constant.h"
#include "advertising.h"
#include "common/systemutils.h"
#include "common/mydebug.h"

Advertising::Advertising()
{
    init();
    MY_SYSTEM(QString("ad Init size=%1").arg(listSize));
}

Advertising* Advertising::getInstance()
{
    static Advertising instance;
    return &instance;
}

void Advertising::init()
{
    lock();
    this->list.clear();
    this->listSize = 0;
    readXML();
    unlock();
}

QString Advertising::get(int32 index)
{
    lock();
    if(!this->list.isEmpty()) {
        if(index >= this->listSize) {
            unlock();
            return NULL;
        }
        unlock();
        return this->list[index];
    }
    unlock();
    return NULL;
}

int32 Advertising::size()
{
    lock();
    int32 size = this->listSize;
    unlock();
    return size;
}

bool Advertising::readXML()
{
    QDomDocument dom;
    QFile *file=new QFile(APP_FILE_PATH+"advertising.xml");
    if(file->open(QIODevice::ReadOnly)) {
        if(!dom.setContent(file)) { //将该xml文件导入到dom中
            file->close();
            MY_WARNING("advertising.xml dom fail");
            return false;
        }
    } else {
        MY_WARNING("advertising.xml open fail");
        return false;
    }
    file->close();

    QDomElement docElem = dom.documentElement();    //返回根元素
    QDomNode node = docElem.firstChild();           //返回根节点的第一个子节点
    while(!node.isNull()) {                         //如果节点不为空
        if(node.isElement()) {                      //如果节点是元素
            QDomElement element=node.toElement();   //将其转化为元素
            QDomNodeList list=element.childNodes(); //取出该元素的所有子节点放到list中

            QString info;
            //将子节点元素全部取出来
            for(int32 i = 0;i < list.count();i++) {
                QDomNode domNode = list.at(i);
                if(domNode.isElement()) {
                    //取出我们所要的数据
                    switch(i) {
                    case 0:
                        info = domNode.toElement().text();//SystemUtils::utf8ToUnicode(domNode.toElement().text());
                        break;
                    }
                }
            }
            this->list.append(info);
            this->listSize++;
        }

        node = node.nextSibling(); //下一个兄弟节点
    }

    return this->listSize > 0 ? true : false;
}

void Advertising::lock()
{
    mutex.lock();
}

void Advertising::unlock()
{
    mutex.unlock();
}
