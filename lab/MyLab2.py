#-*- coding: utf8 -*-
import urllib
import urllib2
import re
import os


list_of_extensions = ['gif', 'bmp', 'jpg', 'jpeg', 'png', 'js', 'css', 'html', 'jsp', 'scn', 'ico']
my_dir = os.getcwd()


url = 'http://lenta.ru'
extra_content = ''
nameTheFolder = url[url.find('/') + 2 : ] + '_files' #создаем имя для папки, куда будем сохранять
os.mkdir(my_dir + '\\' + nameTheFolder)
os.chdir(my_dir + '\\' + nameTheFolder)

content = urllib2.urlopen(url).read()
img_urls = re.findall('img.*?src="(.*?)"', content)
img_urls1 = re.findall('href="(.*?)"', content)
js_urls = re.findall('script.*?src=\"(.*?.js)\"', content)
js_urls1 = re.findall('link.*?href=\"(.*?.js)\"', content)
css_urls = re.findall('link.*?href=\"(.*?.css)\"',content)
urls = img_urls + js_urls + css_urls + js_urls1 + img_urls1
   
for i in range(len(urls)):
    try:
        address = urls[i]
        if address[address.rfind('.') + 1 : ] in list_of_extensions:
                name_file = address[address.rfind('/') + 1 : ]
                #print address
                index = content.find(address)
                extra_content = content[ : index] + './' + name_file + content[index + len(address):]
                content = extra_content
                extra_content = ''
                if address.find('http') < 0:
                    address = url + address
                if address.find('http') > 0:
                    address = address[address.find('http') : ]
                urllib.urlretrieve(address, name_file)
    except  IOError:
        print 'Error'
fout = open(url[url.find('//') + 2 : ] + '.html', 'w')
fout.write(content)
fout.close()
print 'end'
