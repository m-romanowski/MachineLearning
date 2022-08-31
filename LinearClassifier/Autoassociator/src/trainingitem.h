#ifndef TRAININGITEM_H
#define TRAININGITEM_H

#include <QImage>
#include <QColor>
#include <QDebug>

#include <vector>

class TrainingItem
{
public:
    TrainingItem();
    TrainingItem(bool* _data, unsigned int _size);
    ~TrainingItem();

    // Getters
    [[nodiscard]] bool* getData() const { return data; }
    [[nodiscard]] int getLabel() const { return label; }
    [[nodiscard]] unsigned int getSize() const { return size; }

    // Setters
    void setData(bool* _data, unsigned int _size) { data = _data; size = _size; }
    void setLabel(const int _label) { label = _label; }

    static bool* toBits(QImage* _image);

    friend QDebug operator <<(QDebug _dbg, const TrainingItem& _item)
    {
        if(_item.size > 0)
        {
            _dbg.nospace() << _item.label << ": ";

            for(unsigned int i = 0; i < _item.size; ++i)
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
    unsigned int size;
    int label;
};

#endif // TRAININGITEM_H
