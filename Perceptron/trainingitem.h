#ifndef ITEMTOLEARN_H
#define ITEMTOLEARN_H

#include <QImage>
#include <QColor>
#include <QDebug>

#include <vector>

class TrainingItem
{
public:
    TrainingItem();
    ~TrainingItem();

    // Getters
    bool* getData() const { return data; }
    int getLabel() const { return label; }
    int getSize() const { return size; }

    // Setters
    void setData(bool* _data) { data = _data; }
    void setLabel(const int _label) { label = _label; }
    void setSize(int _size) { size = _size; }

    void toBits(QImage* _image);

    friend QDebug operator<< (QDebug _dbg, const TrainingItem& _item)
    {
        if(_item.size > 0)
        {
            _dbg.nospace() << _item.label << ": ";

            for(int i = 0; i < _item.size; ++i)
            {
                if(_item.data[i])
                    _dbg.nospace() << 1 << " ";
                else
                    _dbg.nospace() << 0 << " ";
            }
        }

        return _dbg.maybeSpace();
    }
private:
    bool* data;
    int size;
    int label;
};

#endif // ITEMTOLEARN_H
