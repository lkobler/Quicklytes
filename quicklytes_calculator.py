from tkinter import *
import math

def lcm(a,b):
    i = 1
    ans = b*i
    while(True):
        i+=1
        ans = b*i
        if(ans%a==0):
            return ans


def Nernst(a_con,c_con,a_met,c_met,T,n):
    pot = c_met - a_met
    ans = a_con/c_con
    ans = math.log(ans)
    ans*=(8.314*T)
    ans/=(96485*n)
    return pot-ans




#print(Nernst(1,1,0.34,-0.76,298,2))

master = Tk()
master.title('Calculator')
#intro = Label(master, text = 'Nernst equation calculation')
an_pre = Label(master, text = 'Standard Reduction Potential Anode:')
anode_e = Entry(master,width=10)
cat_pre = Label(master, text = 'Standard Reduction Potential Cathode:')
cathode_e = Entry(master,width=10)
temp = Label(master, text = 'Temp(kelvin):')
temp_e = Entry(master,width=10)
mes = Label(master, text = 'A maximum of four reactant and product concentrations\n can be entered:The small boxes are for powers')
me1 = Label(master, text = 'Num. reactant 1')
e1 = Entry(master, width = 10)
e11 = Entry(master, width = 5)
me2 = Label(master, text = 'Num. reactant 2')
e2 = Entry(master, width = 10)
e22 = Entry(master, width = 5)
me3 = Label(master, text = 'Num. reactant 3')
e3 = Entry(master, width = 10)
e33 = Entry(master, width = 5)
me4 = Label(master, text = 'Num. reactant 4')
e4 = Entry(master, width = 10)
e44 = Entry(master, width = 5)
me5 = Label(master, text = 'Den. reactant 1')
e5 = Entry(master, width = 10)
e55 = Entry(master, width = 5)
me6 = Label(master, text = 'Den. reactant 2')
e6 = Entry(master, width = 10)
e66 = Entry(master, width = 5)
me7 = Label(master, text = 'Den. reactant 3')
e7 = Entry(master, width = 10)
e77 = Entry(master, width = 5)
me8 = Label(master, text = 'Den. reactant 4')
e8 = Entry(master, width = 10)
e88 = Entry(master, width = 5)
num = Label(master, text = 'Number of electrons')
num_e = Entry(master,width=10)
#c_con = Label(master, text = 'Concentration of species at Cathode (moles):')
#c_con_e = Entry(master,width=10)
#a_con = Label(master, text = 'Concentration of species at Anode (moles):')
#a_con_e = Entry(master,width=10)
#oxi_c = Label(master, text = 'oxidation state of cathode \n(absolute value) ')
#oxi_c_e= Entry(master,width=10)
#oxi_a = Label(master, text = 'oxidation state of anode \n(absolute value) ')
#oxi_a_e= Entry(master,width=10)

def click():
    s = ''
    try:
        pot = float(cathode_e.get()) - float(anode_e.get())
        num = (float(e1.get())**float(e11.get()))*(float(e2.get())**float(e22.get()))*(float(e3.get())**float(e33.get()))*(float(e4.get())**float(e44.get()))
        den = (float(e5.get())**float(e55.get()))*(float(e6.get())**float(e66.get()))*(float(e7.get())**float(e77.get()))*(float(e8.get())**float(e88.get()))
        ans = math.log(num/den)
        ans/=((96485)*(float(num_e.get())))
        ans*=((8.314)*(float(temp_e.get())))
        s = str(pot-ans)
    except (TypeError, ZeroDivisionError, ValueError) as e:
        s = str(e)
    final = Tk()
    answer = Label(final, text = s)
    answer.pack()
    mainloop()
#    try:
#        res = Nernst(float(a_con_e.get()), float(c_con_e.get()), float(anode_e.get()), float(cathode_e.get()), float(temp_e.get()), float(num_e.get()))
#    except (TypeError,ZeroDivisionError,ValueError) as s:
#        res = 'Invalid Input: ' + str(s)
#    ans = Tk()
#    ans.title('Potential')
#    answer = Label(ans, text=str(res))
#   answer.pack()





    


#intro.grid(row = 0, column = 2)
mes.grid(row=5, column=1)
an_pre.grid(row =2, column = 0)
anode_e.grid(row =2, column =2)
cat_pre.grid(row=3, column =0)
cathode_e.grid(row=3, column=2)
temp.grid(row=4,column=0)
temp_e.grid(row=4,column=2)
me1.grid(row=6, column=0)
e1.grid(row=6, column=1)
e11.grid(row=6, column=2)
me2.grid(row=7, column=0)
e2.grid(row=7, column=1)
e22.grid(row=7, column=2)
me3.grid(row=8, column=0)
e3.grid(row=8, column=1)
e33.grid(row=8, column=2)
me4.grid(row=9, column=0)
e4.grid(row=9, column=1)
e44.grid(row=9, column=2)
me5.grid(row=10, column=0)
e5.grid(row=10, column=1)
e55.grid(row=10, column=2)
me6.grid(row=11, column=0)
e6.grid(row=11, column=1)
e66.grid(row=11, column=2)
me7.grid(row=12, column=0)
e7.grid(row=12, column=1)
e77.grid(row=12, column=2)
me8.grid(row=13, column=0)
e8.grid(row=13, column=1)
e88.grid(row=13, column=2)
#c_con.grid(row=3, column=4)
#c_con_e.grid(row=3, column=6)
#a_con.grid(row=2,column=4)
#a_con_e.grid(row=2,column=6)
num.grid(row=14, column=0)
num_e.grid(row=14, column=1)
#oxi_c.grid(row=5, column=0)
#oxi_c_e.grid(row=5, column=2)
#oxi_a.grid(row=5, column=4)
#oxi_a_e.grid(row=5, column=6)


Calculate = Button(master, text = 'Calculate', fg = 'blue', bg = 'red', command=click).grid(row = 15, column = 1)
mainloop()


