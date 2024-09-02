#coding=utf8
import socket
import sys
import ConfigParser
import os
import urllib2

remote_ip=''
port=0
RECBUFFER=0
SNDBUFFER=0
DIRNAME=os.path.dirname(os.path.realpath(__file__))
#返回与server端的连接s
def GetConnection():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error, msg:
        print 'Failed to create socket. Error code: ' + str(msg[0]) + ' , Error message : ' + msg[1]
        sys.exit()
    # print 'Socket Created'
    try:
        global remote_ip
        global port
        global RECBUFFER
        global SNDBUFFER
        config = ConfigParser.ConfigParser()
        config.readfp(open(DIRNAME+'/client.ini', "rb"))
        remote_ip=config.get("server", "IP")
        port=int(config.get("server", "PORT"))
        RECBUFFER = int(config.get("client", "RECBUFFER"))
        SNDBUFFER = int(config.get("client", "SNDBUFFER"))
    except Exception,e:
        print e
        print 'Read config file ERROR! (\"client.ini\")'
        sys.exit()
    s.setsockopt(socket.SOL_TCP, socket.TCP_NODELAY, 1)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, SNDBUFFER)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, RECBUFFER)
    s.connect((remote_ip, port))
    # print 'Socket Connected to '+ remote_ip
    return s

#给定查询名称和参数，返回查询结果
def QueryRequest_old(rname,args):
    s=GetConnection()
    cmd=rname
    for arg in args:
        cmd=cmd+' '+str(arg)
    s.sendall(cmd)      #把命令发送给对端
    package_len=102400
    data=''
    # while data.__len__()%package_len==0:
    while 1:
        str_rec=s.recv(package_len)
        if str_rec=='Sending succeeded!':
            break
        data=data+ str_rec#把接收的数据定义为变量
        s.sendall('ack! package received.')

    #print data         #输出变量
    lines=data.split('\n')
    res=[]
    for line in lines:
        splt=line.split()
        if splt.__len__()==1:
            splt=splt[0]
        res.append(splt)
    s.close()   #关闭连接
    # print 'Socket Closed'
    # print 'Get ',res.__len__(),' Lines Records'
    return res

def QueryRequest(rname,args):
    try:
        config = ConfigParser.ConfigParser()
        config.readfp(open(DIRNAME+'/client.ini', "rb"))
        remote_ip=config.get("server", "IP")
        port=config.get("server", "PORT")
    except Exception,e:
        print e
        print 'Read config file ERROR! (\"client.ini\")'
        sys.exit()
    cmd = rname
    for arg in args:
        cmd = cmd + '%20' + str(arg)
    URL='http://'+remote_ip+':'+port+'/'+cmd
    response = urllib2.urlopen(url=URL)
    html = response.read()
    if html[0:2]=='No':
        return []
    lines=html.split('\n')
    res=[]
    for line in lines:
        splt=line.split()
        if splt.__len__()==1:
            splt=splt[0]
        res.append(splt)
    return res

#给定股票代码、时间区间，返回查询结果，ex参数表示是否动态复权，re表示是否复权
def GetStkPrice(stkcode,starttime,endtime,ex=False,re=True):
    res=QueryRequest('GetStkPrice',[stkcode,starttime,endtime])
    del_list=[]
    for i in range(res.__len__()):
        if float(res[i][2])==0:
            del_list.append(i)
    for i in reversed(del_list):
        del(res[i])
    #请求复权因子数据
    ex_res = QueryRequest('GetStkEx', [stkcode])
    # ex等于True时把结束时间以后的复权因子删除
    if ex_res.__len__()==0 or re==False: return res
    if ex==True:
        for index,value in enumerate(ex_res):
            if int(value[1])>endtime:
                ex_res=ex_res[:index]
                break
    # 进行复权计算
    if ex_res.__len__()==0: return res
    for i in range(res.__len__()):

        if res[i][1] >= ex_res[-1][1]:
            break
        # for ex_line in reversed(ex_res):
        for ex_line in ex_res:
            if ex_line[1] > res[i][1]:
                for j in range(4):
                    res[i][2 + j] = str(((float(res[i][2 + j]) - float(ex_line[2])) / (1 + float(ex_line[3]))))
                res[i][6]=str(float(res[i][6])*(1 + float(ex_line[3])))
            # else:
            #     break
    return res

#给定股票代码，获取除权信息
def GetStkEx(stkcode):
    res=QueryRequest('GetStkEx', [stkcode])
    return res

#给定股指代码、时间区间，返回查询结果
def GetInxPrice(inxcode,starttime,endtime):
    return QueryRequest('GetInxPrice', [inxcode, starttime, endtime])

#给定代码，返回中文名称
def GetStkName(stkcode):
    return QueryRequest('GetStkName', [stkcode])

def GetStkCodeST22047():
    return QueryRequest('GetStkCodeST22047',[])

def GetL2Data(stkcode):
    qres= QueryRequest('GetL2Data',[stkcode])
    tmps=[]
    for line in qres:
        try:
            tmp = []
            tmp.append(line[0])
            tmp.append(float(line[1]))
            if tmp[-1] == 0.0: continue
            tmp.append(float(line[2]))
            tmp.append(float(line[3]))
            tmp.append(float(line[4]))
            tmp.append(float(line[5]))
            tmp.append(float(line[6]))
            tmp.append(float(line[7]))
            tmp.append(float(line[8]))
            tmp.append(float(line[9]))
            tmp.append(int(line[10]))
            tmp.append(float(line[11]))
            tmp.append(int(line[12]))
            tmp.append(float(line[13]))
            tmp.append(int(line[14]))
            tmp.append(float(line[15]))
            tmp.append(int(line[16]))
            tmp.append(float(line[17]))
            tmp.append(int(line[18]))
            tmp.append(float(line[19]))
            tmp.append(int(line[20]))
            tmp.append(float(line[21]))
            tmp.append(int(line[22]))
            tmp.append(float(line[23]))
            tmp.append(int(line[24]))
            tmp.append(float(line[25]))
            tmp.append(int(line[26]))
            tmp.append(float(line[27]))
            tmp.append(int(line[28]))
            tmp.append(float(line[29]))
            tmp.append(int(line[30][0:4])*10000000000+int(line[30][5:7])*100000000+int(line[30][8:10])*1000000+int(line[31][0:2])*10000+int(line[31][3:5])*100+int(line[31][6:8])*1)
            tmps.append(tmp)
        except Exception,e:
            print e
            print line
        #print tmp
    return tmps

def GetStkCode():
    return QueryRequest('GetStkCode',[])

# codes= GetStkCode()
def GetTimeStr(daytime,ttime=-1,template='--::'):
    if ttime==-1:
        year=str(daytime/10000000000)
        month=str((daytime%10000000000)/100000000)
        day=str((daytime%100000000)/1000000)
        hour=str((daytime%1000000)/10000)
        minute=str((daytime%10000)/100)
        second=str(daytime%100)
    else:
        year = str(daytime / 10000)
        month = str((daytime % 10000) / 100)
        day = str((daytime % 100) / 1)
        hour = str(ttime / 10000)
        minute = str((ttime % 10000) / 100)
        second = str(ttime % 100)
    if month.__len__() == 1: month = '0' + month
    if day.__len__() == 1: day = '0' + day
    if hour.__len__() == 1: hour = '0' + hour
    if minute.__len__() == 1: minute = '0' + minute
    if second.__len__() == 1: second = '0' + second
    str1 = year + template[0] + month + template[1] + day
    str2 = hour + template[2] + minute + template[3] + second
    return {'day': str1, 'time': str2}


def Stradge001(code,timelen=10,dis=0.01,tcnt=5):
    print 'stockcode=',code
    data=GetL2Data(code)
    if data.__len__()==0: return []
    his = []
    tbuy = 0
    tsell = 0
    tcp = 0
    lday=0
    found=0
    bp=0
    res=[]
    rec=[]
    for line in data:
        day=int(line[-1]/1000000)
        time=line[-1]%1000000
        if time>=150000 or time<=93000 or time>=113000 and time <=130000:
            continue
        elif time>=145700 and (code[2]=='0' or code[2]=='3'):
            continue
        if lday<>day:
            his = []
            tbuy = 0
            tsell = 0
            tcp = 0
            lday=day
            if found==1:
                rec.append(line[1])
                rec.append((line[1]-bp)/bp)
                found = 0
                bp = 0
                if abs(rec[-1])<0.2:
                    res.append(rec)
                    print 'Next day open = ', line[1], ', up ', rec[-1]*100, '%'
                    #print line
        buy=line[10]+line[12]+line[14]+line[16]+line[18]
        sell=line[20]+line[22]+line[24]+line[26]+line[28]
        cp=line[3]
        tbuy=tbuy+buy
        tsell=tsell+sell
        tcp=tcp+cp
        his.append([buy,sell,cp])

        if his.__len__()>timelen:
            tbuy=tbuy-his[0][0]
            tsell=tsell-his[0][1]
            tcp=tcp-his[0][2]
            his=his[1:]
            # p= tcp/timelen
            # t=(his[0][2]+his[-1][2])/2
            maxp = 0.001
            minp = 9999.99
            for hi in his:
                if hi[2] > maxp:
                    maxp = hi[2]
                if hi[2] < minp:
                    minp = hi[2]
            #print tbuy, ', ', tsell, ', ', p,', ',t
            if sell*0.95<line[20] or buy*1000<sell:
                continue
            if (maxp-minp)/minp<dis and tbuy*tcnt<tsell and found==0:
                bp = line[3]
                print his[-1]
                timestr=GetTimeStr(day,time)
                print timestr['day'],' ',timestr['time'],': buy at ',bp
                found=1
                rec=[line[-1],bp]
    return  res

def Stradge001Test(maxlen=100,timelen=10,dis=0.01,tcnt=5):
    codes=GetStkCode()
    res={}
    for code in codes:
        print code
        if code[2]<>'0' and code[2]<>'6' and code[2]<>'3':continue
        if res.__len__()==maxlen: return res
        tmp=Stradge001(code,timelen,dis,tcnt)
        if tmp.__len__()==0: continue
        res[code]={'data':tmp}
        win=0
        lost=0
        aver=0
        for line in tmp:
            if line[3]<0:
                lost=lost+1
            else:
                win=win+1
            aver=aver+line[3]
        res[code]['code']=code
        res[code]['win']=win
        res[code]['lost']=lost
        res[code]['aver']=aver
    return res







#
# res= GetStkPrice('SH600050.stk',20100000000000,20170401000000,True)
# for line in res:
#     print line

# print GetStkName('SH000300.index')




