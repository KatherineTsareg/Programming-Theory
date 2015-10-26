#-*- coding: utf8 -*-
import urllib
import urllib2
import re
import os

list_of_extensions = ['gif', 'bmp', 'jpg', 'jpeg', 'png', 'js', 'css', 'html', 'ico']
my_dir = os.getcwd()
numb_of_pages = 100


def SaveRecourcesUrl(main_url, url, count, my_dir):
    extra_content = ''
    nameTheFolder = str(count) #создаем имя для папки, куда будем сохранять
    new_dir = my_dir + '\\' + nameTheFolder
    os.mkdir(new_dir)
    os.chdir(new_dir)
    if url.find('http') < 0:
        correct_url = main_url + url
    else:
        correct_url = url
    content = urllib2.urlopen(correct_url).read()
    img_urls = re.findall('img.*?src="(.*?)"', content)
    img_urls1 = re.findall('href="(.*?)"', content)
    js_urls = re.findall('script.*?src=\"(.*?.js)\"', content)
    js_urls1 = re.findall('link.*?href=\"(.*?.js)\"', content)
    css_urls = re.findall('link.*?href=\"(.*?.css)\"',content)
    urls = img_urls + js_urls + css_urls + js_urls1 + img_urls1
    #print urls
    for i in range(len(urls)):
        try:
            address = urls[i]
            if address[address.rfind('.') + 1 : ] in list_of_extensions:
                name_file = address[address.rfind('/') + 1 : ]
                index = content.find(address)
                extra_content = content[ : index] + './' + name_file + content[index + len(address):]
                content = extra_content
                extra_content = ''
                if address.find('http') < 0:
                    address = main_url + address
                if address.find('http') > 0:
                    address = address[address.find('http') : ]
                urllib.urlretrieve(address, name_file)
        except  IOError:
            print 'Error'
    fout = open(str(count) + '.html', 'w')
    fout.write(content)
    fout.close()


      
urls_list = []
savedPages = []
count = 1
working = True

main_url = 'http://lenta.ru'
word = 'Медведев'
content = urllib2.urlopen(main_url).read()
urls_list = re.findall('a.*?href="(.*?)"',content)
i = 0
while (i < len(urls_list)) and (len(savedPages) <= numb_of_pages):
    url = urls_list[i]
    if url.find('http') < 0:
        correct_url = main_url + url
    else:
        correct_url = url
    if (url.find('@') > 0):
        urls_list.pop(i)
        working = False
    if working:
        #print correct_url
        content = urllib2.urlopen(correct_url).read()
        if (content.find(word) > 0) and (correct_url not in savedPages) and (len(savedPages) <= numb_of_pages) and (correct_url != (main_url +'/rss')):
            try:
                SaveRecourcesUrl(main_url, url, count, my_dir)
                os.chdir(my_dir)
                count += 1
                savedPages.append(correct_url)
                if len(urls_list) <= 1000:
                    urls_list += re.findall('a.*?href="(.*?)"',content)
                urls_list.pop(i)
            except  IOError:
                urls_list.pop(i)
        elif (content.find(word) < 0) and (correct_url not in savedPages):
            if len(urls_list) <= 1000:
                urls_list += re.findall('a.*?href="(.*?)"',content)
            urls_list.pop(i)
        elif ((content.find(word) > 0) and (correct_url in savedPages)) or (correct_url == (main_url +'/rss')):
            urls_list.pop(i)
    working = True
print 'End'

