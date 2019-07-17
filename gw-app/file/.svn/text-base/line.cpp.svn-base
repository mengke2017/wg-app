#include <QtXml>
#include <QFile>
#include <QIODevice>
#include <QDomNodeList>
#include "line.h"
#include "constant.h"
#include "common/mydebug.h"
#include "common/systemutils.h"

Line::Line()
{
    init();
    MY_SYSTEM(QString("line Init size=%1").arg(listSize));
}

Line* Line::getInstance()
{
    static Line instance;
    return &instance;
}

void Line::init()
{
    lock();
    this->list.clear();
    this->listSize = 0;
    readXML();
    unlock();
}

void Line::get(int32 index,line_info* info)
{
    lock();
    if(!this->list.isEmpty()) {
        if(index < this->listSize) {
            info->name = this->list[index].name;
            info->first_up = this->list[index].first_up;
            info->end_up = this->list[index].end_up;
            info->lines_up = this->list[index].lines_up;
            info->first_down = this->list[index].first_down;
            info->end_down = this->list[index].end_down;
            info->lines_down = this->list[index].lines_down;
        }
    }
    unlock();
}

int32 Line::size()
{
    lock();
    int32 size = this->listSize;
    unlock();
    return size;
}

bool Line::readXML()
{
    QDomDocument dom;
    QFile *file=new QFile(APP_FILE_PATH+"line.xml");
    if(file->open(QIODevice::ReadOnly)) {
        if(!dom.setContent(file)) { //将该xml文件导入到dom中
            file->close();
            MY_WARNING("line.xml dom fail");
            return false;
        }
    } else {
        MY_WARNING("line.xml open fail");
        return false;
    }
    file->close();

    QDomElement docElem = dom.documentElement();    //返回根元素
    QDomNode node = docElem.firstChild();           //返回根节点的第一个子节点
    while(!node.isNull()) {                         //如果节点不为空
        if(node.isElement()) {                      //如果节点是元素
            QDomElement element=node.toElement();   //将其转化为元素
            QDomNodeList list=element.childNodes(); //取出该元素的所有子节点放到list中

            line_info info;
            //将子节点元素全部取出来
            for(int32 i = 0;i < list.count();i++) {
                QDomNode domNode = list.at(i);
                if(domNode.isElement()) {
                    //取出我们所要的数据
                    switch(i) {
                    case 0:
                        info.name = domNode.toElement().text();
                        break;

                    case 1:
                        info.first_up = domNode.toElement().text();
                        break;

                    case 2:
                        info.end_up = domNode.toElement().text();
                        break;

                    case 3:
                        info.lines_up = domNode.toElement().text();
                        break;

                    case 4:
                        info.first_down = domNode.toElement().text();
                        break;

                    case 5:
                        info.end_down = domNode.toElement().text();
                        break;

                    case 6:
                        info.lines_down = domNode.toElement().text();
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

void Line::lock()
{
    mutex.lock();
}

void Line::unlock()
{
    mutex.unlock();
}
