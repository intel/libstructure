#include "include/import.hpp"

#include "Visitor.hpp"
#include "client/type.hpp"
#include "ValueImporter.hpp"

#include <utility>

namespace structure
{

class ImportVisitor : public StructureVisitor
{
public:
    ImportVisitor(ValueImporter &mImporter) : mImporter(mImporter), mPath("/") {}

    void visit(const Block &block) override
    {
        std::unique_ptr<BlockValue> blockValue = std::make_unique<BlockValue>(block);
        std::string savedPath = mPath;
        mPath = mPath + block.getName() + "/";

        bool savedFirst = mFirst;
        mFirst = false;

        for (const auto &field : block.getFields()) {
            field.get().accept(*this);
            blockValue->addValue(std::move(mSubValue));
        }

        mSubValue = std::move(blockValue);

        if (savedFirst) {
            mRoot = std::move(mSubValue);
        }

        mPath = savedPath;
    }

    void visit(const GenericField &f) override
    {
        mSubValue = mImporter.import(f, mPath + f.getName());

        if (mFirst) {
            mRoot = std::move(mSubValue);
            mFirst = false;
        }
    }

    std::unique_ptr<StructureValue> getValue() { return std::move(mRoot); }

private:
    bool mFirst = true;
    std::unique_ptr<StructureValue> mSubValue;
    std::unique_ptr<StructureValue> mRoot;
    ValueImporter &mImporter;
    std::string mPath;
};

std::unique_ptr<StructureValue> importValue(const Structure &structure, ValueImporter &mImporter)
{
    ImportVisitor import(mImporter);
    structure.accept(import);

    return import.getValue();
}
}