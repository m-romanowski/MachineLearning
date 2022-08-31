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
    [[nodiscard]] double* data() const { return mData; }
    std::vector<double> dataVector();
    [[nodiscard]] int label() const { return mLabel; }
    [[nodiscard]] int size() const { return mSize; }

    // Setters
    void setData(double* _data, int _size) { mData = _data; mSize = _size; }
    void setData(std::vector<double>& _data);
    void setLabel(const int _label) { mLabel = _label; }

    void toBits(QImage* _image);

    friend QDebug operator<< (QDebug _dbg, const TrainingItem& _item)
    {
        if(_item.mSize > 0)
        {
            _dbg.nospace() << _item.mLabel << ": ";

            for(int i = 0; i < _item.mSize; ++i)
                _dbg.nospace() << _item.mData[i] << " ";
        }

        return _dbg.maybeSpace();
    }
private:
    double* mData;
    int mSize;
    int mLabel;
};

#endif // ITEMTOLEARN_H
