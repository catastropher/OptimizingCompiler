#pragma once

#include <map>

struct Polynomial
{
    Polynomial(int val)
    {
        coeff["constant"] = val;
    }
    
    Polynomial(std::string var)
    {
        coeff[var] = 1;
    }
    
    bool onlyConstant()
    {
        return coeff.size() == 1 && coeff.count("constant") == 1;
    }
    
    Polynomial add(Polynomial& p)
    {
        Polynomial res = *this;
        for(auto c : p.coeff)
            res.coeff[c.first] += c.second;
        
        return res;
    }
    
    Polynomial sub(Polynomial& p)
    {
        Polynomial res = *this;
        for(auto c : p.coeff)
            res.coeff[c.first] += c.second;
        
        return res;
    }
    
    Polynomial mul(Polynomial& p)
    {
        if(!onlyConstant() && !p.onlyConstant())
            throw "Bad polynomial mul";
        
        if(p.onlyConstant())
        {
            Polynomial res = *this;
            for(auto c : coeff)
            {
                res.coeff[c.first] *= p.coeff["constant"];
            }
            
            
            return res;
        }
        else
            return p.mul(*this);
    }
    
    Polynomial neg()
    {
        Polynomial res = *this;
        for(auto c : coeff)
            res.coeff[c.first] = -res.coeff[c.first];
        
        return res;
    }
    
    void print()
    {
        bool first = true;
        
        for(auto c : coeff)
        {
            if(!first)
                printf(" + ");
            
            first = false;
            printf("%d * %s", c.second, c.first.c_str());
        }
        
        printf("\n");
    }
    
    std::map<std::string, int> coeff;
};

