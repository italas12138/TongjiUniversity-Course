#coding=utf-8
import urllib2
import MakeStockCode as ms


# 从文件读取所有股票代码，返回两个list，一个只包含6位数字，一个有前后缀
def get_all_stkcode(read_file_name):
    file_r = open(read_file_name, 'r')
    count = 0
    read_str = file_r.read()
    r1=[]
    r2=[]
    sb='SH'
    for i in range(read_str.__len__()):
        if read_str[i]=='!':sb='SZ'
        if read_str[i]=='(':
            tmp_str=read_str[i+1:i+7]
            r1.append(tmp_str)
            r2.append(sb+tmp_str+'.stk')
    return r1,r2

def get_urls_by_file(read_file_name,write_file_name):
    url_len=200     #单个URL包含的股票代码数量
    file_r=open(read_file_name,'r')
    urls=[]
    #读取上海股票代码
    read_str=file_r.readline()
    url='http://hq.sinajs.cn/list='
    count=0
    for i in range(read_str.__len__()):
        if read_str[i]=='(':
            tmp_str='sh'+read_str[i+1:i+7]
            response = urllib2.urlopen('http://hq.sinajs.cn/list='+tmp_str)
            html = response.read()
            print 'request '+str(count)+" done!"
            if len(html)>30:
                if count==url_len:
                    # print url[:-1]
                    urls.append(url[:-1])
                    count=0
                    url='http://hq.sinajs.cn/list='
                url=url+tmp_str+','
                count=count+1
            # if count == 200:
            # # print url[:-1]
            #     urls.append(url[:-1])
            #     count=0
            #     url='http://hq.sinajs.cn/list='
            # url=url+tmp_str+','
            # count=count+1
            i=i+7
    # print url[:-1]
    urls.append(url[:-1])

    #读取空行
    read_str=file_r.readline()

    #读取深圳股票代码
    read_str=file_r.readline()
    url='http://hq.sinajs.cn/list='
    count=0
    for i in range(read_str.__len__()):
        if read_str[i]=='(':
            tmp_str = 'sz' + read_str[i + 1:i + 7]
            response = urllib2.urlopen('http://hq.sinajs.cn/list=' + tmp_str)
            html = response.read()
            if len(html) > 30:
                if count==url_len:
                    # print url[:-1]
                    urls.append(url[:-1])
                    count=0
                    url='http://hq.sinajs.cn/list='
                url=url+'sz'+read_str[i+1:i+7]+','
                count=count+1
            i=i+7
    # print url[:-1]
    urls.append(url[:-1])
    # print urls
    # print urls.__len__()
    # print urls[43]
    file_w=open(write_file_name,'w')
    for i in range(urls.__len__()):
        write_str=urls[i]+'\n'
        file_w.write(write_str)
    file_r.close()
    file_r=open('indexURLs.txt')
    print 'open file!'
    for line in file_r.readlines():
        file_w.write(line)
        print line
    file_w.close()
    return urls

def get_urls_by_web(sc_file,urls_file):
    ms.get_stock_code(sc_file)
    get_urls_by_file(sc_file,urls_file)
