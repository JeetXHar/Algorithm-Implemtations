from re import S
import sys
sys.stdin=open('iris.data','r')
import random as rd
from math import exp

class Multilayer_perseptron:

    def __init__(self,eta,N,M,O):
        """initialize the Multilayer Perseptron model"""
        self.N=N
        self.M=M
        self.O=O
        self.middlelayer=[1]*M
        self.Y=[0]*O
        self.w1=[[0 for j in range(N)] for i in range(M)]
        self.w2=[[0 for j in range(M)] for i in range(O)]
        self.err=[0]*O
        self.eta=eta

    def sigmoid(self,x):
        """Calculate sigmoid of input"""
        return exp(x)/(1+exp(x))
    
    def predict(self,Input,output):
        """Make Pridiction on `Input` data"""
        self.input=Input
        self.output=output

        for j in range(1,self.M):
            self.middlelayer[j]=0
            for i in range(self.N):
                self.middlelayer[j]+=self.w1[j][i]*self.input[i]
                # print(self.w1[j][i],self.input[i])
            self.middlelayer[j]=self.sigmoid(self.middlelayer[j])

        for j in range(self.O):
            self.Y[j]=0
            for i in range(self.M):
                self.Y[j]+=self.w2[j][i]*self.middlelayer[i]
            self.Y[j]=self.sigmoid(self.Y[j])

        mx=-1;res=0

        for i in range(self.O):
            if self.Y[i]>mx:
                mx=self.Y[i]
                res=i
        return res

    def Error(self):
        """Calculate Error on output"""
        for i in range(self.O):
            self.err[i]=self.output[i]-self.Y[i]
    
    def backPropagation(self):
        """Train model on last feeded data"""
        delta1=[0]*self.O
        delta2=[0]*self.M

        for i in range(self.O):
            delta1[i]=self.err[i]*self.Y[i]*(1-self.Y[i])
        
        delta2=[self.middlelayer[j]*(1-self.middlelayer[j])*sum([delta1[k]*self.w2[k][j] for k in range(self.O)]) for j in range(self.M)]
        
        for i in range(self.O):
            for j in range(self.M):
                self.w2[i][j]+=self.eta*delta1[i]*self.middlelayer[j]
        
        for i in range(self.M):
            for j in range(self.N):
                self.w1[i][j]+=self.eta*delta2[i]*self.input[j]
    
    def printweights(self):
        """Show weight matrixes"""
        for x in self.w1:
            print(x)
        for x in self.w2:
            print(x)
    
def main():
    n=4;m=5;o=3;len=150
    eta=0.02

    mlp=Multilayer_perseptron(eta,n+1,m+1,o)

    data=[input() for i in range(len)]
    rd.shuffle(data)

    Input=[[0]*n for i in range(len)]
    output=[[0]*m for i in range(len)]
    for i,x in enumerate(data):
        t=list(x.strip().split(','))
        Input[i]=[1]+list(map(float,t[:n]))
        if t[-1]=="Iris-setosa":
            output[i]=[1,0,0]
        elif t[-1]=="Iris-versicolor":
            output[i]=[0,1,0]
        else:
            output[i]=[0,0,1]
    for i in range(5000):
        for j in range(len):
            mlp.predict(Input[j],output[j])
            mlp.Error()
            mlp.backPropagation()
    
    accuracy=0
    for i in range(len):
        accuracy+=output[i][mlp.predict(Input[i],output[i])]
    
    print(accuracy*2/3)

    pass
if __name__=="__main__":
    main()