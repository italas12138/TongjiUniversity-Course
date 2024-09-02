#coding=utf-8
import urllib2

def get_stock_code(write_file):
    response = urllib2.urlopen('http://quote.eastmoney.com/stocklist.html')
    print response.headers
    html=response.read().decode('gbk').encode('utf-8')
    lines=html.split('\n')
    w_file=open(write_file,'w')
    sz=''
    sh=''
    for line in lines:
        if line.find('<li><a target=\"_blank\" href=\"http://quote')<>-1:
            code=line[line.find('html')+6:line.find('</a></li>')]
            if line.find('/sh')==-1:
                sz=sz+code+' '
            else:
                sh=sh+code+' '
    w_data=sh+'\n!\n'+sz
    print w_data
    w_file.write(w_data)
    w_file.close()
