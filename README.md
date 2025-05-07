## 部署指南

### 1. 准备工作

1. 安装MySQL服务器:

bash

```
sudo apt-get install mysql-server
```

1. 创建数据库和用户:

sql

```
CREATE USER 'library'@'localhost' IDENTIFIED BY 'password';
GRANT ALL PRIVILEGES ON library.* TO 'library'@'localhost';
FLUSH PRIVILEGES;
```

1. 导入数据库结构:

bash

```
mysql -u root -p library < sql/library.sql
```

### 2. 编译和运行

1. 安装编译依赖:

bash

```
sudo apt-get install build-essential cmake libmysqlclient-dev
```

1. 编译项目:

bash

```
mkdir build && cd build
cmake ..
make
```

1. 运行服务器:

bash

```
./library_server
```

1. 访问系统:
   打开浏览器访问 `http://localhost:8080`

## 功能扩展建议

1. 添加用户认证系统
2. 实现图书搜索功能
3. 添加借阅记录页面
4. 实现图书封面图片上传
5. 添加分页功能
6. 实现JSON API接口

这个项目展示了如何使用C++构建一个完整的Web应用，包括数据库操作、HTTP服务器和简单的前端界面。你可以根据需要进一步扩展功能。