#ifndef ast_default_hpp
#define ast_default_hpp

#include "ast_Node.hpp"
#include <string>


class defaultStatement
    : public DeclarationRoot
{
private:

protected:
    List_Ptr statement;
public:
    defaultStatement(List_Ptr _statement)
    :statement(_statement)
    {}
    virtual ~defaultStatement(){
        // delete branches[0];
        // delete branches[1];
    }

    //virtual const char *getOpcode() const =0;
    /*
    virtual int getSize(Context &context)const{
        for(int i = 0; i < statement->size(); i++){
            statement->at(i)->getSize(context);
        }
        // return statement->getSize(context);
    }
*/
    virtual int getSize() const override{
        int totalSize = 0;
        for (int i = 0; i < statement->size(); i++) {
            // 假设 DerivedNode 是派生自 Node 的类，并实现了 getSize 方法。
            // 尝试将 Node_Ptr 转换为 DerivedNode*。
            const DeclarationRoot* derivedNode = dynamic_cast<const DeclarationRoot*>(statement->at(i));

            if (derivedNode) {
                // 如果转换成功，则调用 DerivedNode 的 getSize 方法。
                totalSize += derivedNode->getSize();
            } else {
                // 如果转换失败，可能需要处理或忽略错误。
                // 这取决于您的错误处理策略。
                // 在这里，我们选择忽略该节点并继续。
                std::cerr << "Warning: Node cannot be cast to DerivedNode." << std::endl;
            }
        }
        return totalSize;
    }

    virtual const std::string& getIdentifier() const override{
        std::cerr<<"Error: getting id of case statement";
        exit(1);
    }
    virtual void Print(std::ostream &dst) const override
    {
        dst<<"default statement: ";
        dst<<"statement:"<<std::endl;
        for(int i = 0; i < statement->size(); i++){
            statement->at(i)->Print(dst);
        }
        // statement->print(dst);
    }

    void EmitRISC(std::ostream &dst, Context &context, int destReg) const override{
        // dst<<context.stack.back().startLabel<<":"<<std::endl;
        for(int i = 0; i < statement->size(); i++){
            statement->at(i)->EmitRISC(dst,context,destReg);
        }
        // statement->EmitRISC(dst,context,destReg);
    }
};

#endif
