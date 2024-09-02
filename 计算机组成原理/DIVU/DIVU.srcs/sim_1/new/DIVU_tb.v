//Course.java

packagecom.hjf.entity;public classCourse {private intid;privateString name;privateString teacher;privateString classroom;public intgetId() {returnid;

}public void setId(intid) {this.id =id;

}publicString getName() {returnname;

}public voidsetName(String name) {this.name =name;

}publicString getTeacher() {returnteacher;

}public voidsetTeacher(String teacher) {this.teacher =teacher;

}publicString getClassroom() {returnclassroom;

}public voidsetClassroom(String classroom) {this.classroom =classroom;

}publicCourse() {}public Course(intid, String name, String teacher, String classroom) {this.id =id;this.name =name;this.teacher =teacher;this.classroom =classroom;

}publicCourse(String name, String teacher, String classroom) {this.name =name;this.teacher =teacher;this.classroom =classroom;

}

}

//CourseDao.java

packagecom.hjf.dao;importjava.sql.Connection;importjava.sql.ResultSet;importjava.sql.SQLException;importjava.sql.Statement;importjava.util.ArrayList;importjava.util.List;importcom.hjf.entity.Course;importcom.hjf.util.DBUtil;/*** 课程Dao

* Dao层操作数据

*@authorHu

**/

public classCourseDao {/*** 添加

*@paramcourse

*@return

*/

public booleanadd(Course course) {

String sql= "insert into course(name, teacher, classroom) values('" + course.getName() + "','" + course.getTeacher() + "','" + course.getClassroom() + "')";//创建数据库链接

Connection conn =DBUtil.getConn();

Statement state= null;boolean f = false;int a = 0;try{

state=conn.createStatement();

state.executeUpdate(sql);

}catch(Exception e) {

e.printStackTrace();

}finally{//关闭连接

DBUtil.close(state, conn);

}if (a > 0) {

f= true;

}returnf;

}/*** 删除

*

*@paramid

*@return

*/

public boolean delete (intid) {boolean f = false;

String sql= "delete from course where id='" + id + "'";

Connection conn=DBUtil.getConn();

Statement state= null;int a = 0;try{

state=conn.createStatement();

a=state.executeUpdate(sql);

}catch(SQLException e) {

e.printStackTrace();

}finally{

DBUtil.close(state, conn);

}if (a > 0) {

f= true;

}returnf;

}/*** 修改

*@paramname

*@parampass*/

public booleanupdate(Course course) {

String sql= "update course set name='" + course.getName() + "', teacher='" + course.getTeacher() + "', classroom='" +course.getClassroom()+ "' where id='" + course.getId() + "'";

Connection conn=DBUtil.getConn();

Statement state= null;boolean f = false;int a = 0;try{

state=conn.createStatement();

a=state.executeUpdate(sql);

}catch(SQLException e) {

e.printStackTrace();

}finally{

DBUtil.close(state, conn);

}if (a > 0) {

f= true;

}returnf;

}/*** 验证课程名称是否唯一

* true --- 不唯一

*@paramname

*@return

*/

public booleanname(String name) {boolean flag = false;

String sql= "select name from course where name = '" + name + "'";

Connection conn=DBUtil.getConn();

Statement state= null;

ResultSet rs= null;try{

state=conn.createStatement();

rs=state.executeQuery(sql);while(rs.next()) {

flag= true;

}

}catch(SQLException e) {

e.printStackTrace();

}finally{

DBUtil.close(rs, state, conn);

}returnflag;

}/*** 通过ID得到课程信息

*@paramid

*@return

*/

public Course getCourseById(intid) {

String sql= "select * from course where id ='" + id + "'";

Connection conn=DBUtil.getConn();

Statement state= null;

ResultSet rs= null;

Course course= null;try{

state=conn.createStatement();

rs=state.executeQuery(sql);while(rs.next()) {

String name= rs.getString("name");

String teacher= rs.getString("teacher");

String classroom= rs.getString("classroom");

course= newCourse(id, name, teacher, classroom);

}

}catch(Exception e) {

e.printStackTrace();

}finally{

DBUtil.close(rs, state, conn);

}returncourse;

}/*** 通过name得到Course

*@paramname

*@return

*/

publicCourse getCourseByName(String name) {

String sql= "select * from course where name ='" + name + "'";

Connection conn=DBUtil.getConn();

Statement state= null;

ResultSet rs= null;

Course course= null;try{

state=conn.createStatement();

rs=state.executeQuery(sql);while(rs.next()) {int id = rs.getInt("id");

String teacher= rs.getString("teacher");

String classroom= rs.getString("classroom");

course= newCourse(id, name, teacher, classroom);

}

}catch(Exception e) {

e.printStackTrace();

}finally{

DBUtil.close(rs, state, conn);

}returncourse;

}/*** 查找

*@paramname

*@paramteacher

*@paramclassroom

*@return

*/

public Listsearch(String name, String teacher, String classroom) {

String sql= "select * from course where ";if (name != "") {

sql+= "name like '%" + name + "%'";

}if (teacher != "") {

sql+= "teacher like '%" + teacher + "%'";

}if (classroom != "") {

sql+= "classroom like '%" + classroom + "%'";

}

List list = new ArrayList<>();

Connection conn=DBUtil.getConn();

Statement state= null;

ResultSet rs= null;try{

state=conn.createStatement();

rs=state.executeQuery(sql);

Course bean= null;while(rs.next()) {int id = rs.getInt("id");

String name2= rs.getString("name");

String teacher2= rs.getString("teacher");

String classroom2= rs.getString("classroom");

bean= newCourse(id, name2, teacher2, classroom2);

list.add(bean);

}

}catch(SQLException e) {

e.printStackTrace();

}finally{

DBUtil.close(rs, state, conn);

}returnlist;

}/*** 全部数据

*@paramname

*@paramteacher

*@paramclassroom

*@return

*/

public Listlist() {

String sql= "select * from course";

List list = new ArrayList<>();

Connection conn=DBUtil.getConn();

Statement state= null;

ResultSet rs= null;try{

state=conn.createStatement();

rs=state.executeQuery(sql);

Course bean= null;while(rs.next()) {int id = rs.getInt("id");

String name2= rs.getString("name");

String teacher2= rs.getString("teacher");

String classroom2= rs.getString("classroom");

bean= newCourse(id, name2, teacher2, classroom2);

list.add(bean);

}

}catch(SQLException e) {

e.printStackTrace();

}finally{

DBUtil.close(rs, state, conn);

}returnlist;

}

}

//CourseService.java

packagecom.hjf.service;importjava.util.List;importcom.hjf.dao.CourseDao;importcom.hjf.entity.Course;/*** CourseService

* 服务层

*@authorHu

**/

public classCourseService {

CourseDao cDao= newCourseDao();/*** 添加

*@paramcourse

*@return

*/

public booleanadd(Course course) {boolean f = false;if(!cDao.name(course.getName())) {

cDao.add(course);

f= true;

}returnf;

}/*** 删除*/

public void del(intid) {

cDao.delete(id);

}/*** 修改

*@return

*/

public voidupdate(Course course) {

cDao.update(course);

}/*** 通过ID得到一个Course

*@return

*/

public Course getCourseById(intid) {returncDao.getCourseById(id);

}/*** 通过Name得到一个Course

*@return

*/

publicCourse getCourseByName(String name) {returncDao.getCourseByName(name);

}/*** 查找

*@return

*/

public Listsearch(String name, String teacher, String classroom) {returncDao.search(name, teacher, classroom);

}/*** 全部数据

*@return

*/

public Listlist() {returncDao.list();

}

}

//CourseServlet.java

packagecom.hjf.servlet;importjava.io.IOException;importjava.util.List;importjavax.servlet.ServletException;importjavax.servlet.annotation.WebServlet;importjavax.servlet.http.HttpServlet;importjavax.servlet.http.HttpServletRequest;importjavax.servlet.http.HttpServletResponse;importcom.hjf.entity.Course;importcom.hjf.service.CourseService;

@WebServlet("/CourseServlet")public class CourseServlet extendsHttpServlet {private static final long serialVersionUID = 1L;

CourseService service= newCourseService();/*** 方法选择*/

protected void service(HttpServletRequest req, HttpServletResponse resp) throwsServletException, IOException {

req.setCharacterEncoding("utf-8");

String method= req.getParameter("method");if ("add".equals(method)) {

add(req, resp);

}else if ("del".equals(method)) {

del(req, resp);

}else if ("update".equals(method)) {

update(req, resp);

}else if ("search".equals(method)) {

search(req, resp);

}else if ("getcoursebyid".equals(method)) {

getCourseById(req, resp);

}else if ("getcoursebyname".equals(method)) {

getCourseByName(req, resp);

}else if ("list".equals(method)) {

list(req, resp);

}

}/*** 添加

*@paramreq

*@paramresp

*@throwsIOException

*@throwsServletException*/

private void add(HttpServletRequest req, HttpServletResponse resp) throwsIOException, ServletException {

req.setCharacterEncoding("utf-8");//获取数据

String name = req.getParameter("name");

String teacher= req.getParameter("teacher");

String classroom= req.getParameter("classroom");

Course course= newCourse(name, teacher, classroom);//添加后消息显示

if(service.add(course)) {

req.setAttribute("message", "添加成功");

req.getRequestDispatcher("add.jsp").forward(req,resp);

}else{

req.setAttribute("message", "课程名称重复，请重新录入");

req.getRequestDispatcher("add.jsp").forward(req,resp);

}

}/*** 全部

*@paramreq

*@paramresp

*@throwsServletException*/

private void list(HttpServletRequest req, HttpServletResponse resp) throwsIOException, ServletException{

req.setCharacterEncoding("utf-8");

List courses =service.list();

req.setAttribute("courses", courses);

req.getRequestDispatcher("list.jsp").forward(req,resp);

}/*** 通过ID得到Course

*@paramreq

*@paramresp

*@throwsServletException*/

private void getCourseById(HttpServletRequest req, HttpServletResponse resp) throwsIOException, ServletException{

req.setCharacterEncoding("utf-8");int id = Integer.parseInt(req.getParameter("id"));

Course course=service.getCourseById(id);

req.setAttribute("course", course);

req.getRequestDispatcher("detail2.jsp").forward(req,resp);

}/*** 通过名字查找

* 跳转至删除

*@paramreq

*@paramresp

*@throwsIOException

*@throwsServletException*/

private void getCourseByName(HttpServletRequest req, HttpServletResponse resp) throwsIOException, ServletException{

req.setCharacterEncoding("utf-8");

String name= req.getParameter("name");

Course course=service.getCourseByName(name);if(course == null) {

req.setAttribute("message", "查无此课程！");

req.getRequestDispatcher("del.jsp").forward(req,resp);

}else{

req.setAttribute("course", course);

req.getRequestDispatcher("detail.jsp").forward(req,resp);

}

}/*** 删除

*@paramreq

*@paramresp

*@throwsIOException

*@throwsServletException*/

private void del(HttpServletRequest req, HttpServletResponse resp) throwsIOException, ServletException{

req.setCharacterEncoding("utf-8");int id = Integer.parseInt(req.getParameter("id"));

service.del(id);

req.setAttribute("message", "删除成功！");

req.getRequestDispatcher("del.jsp").forward(req,resp);

}/*** 修改

*@paramreq

*@paramresp

*@throwsIOException

*@throwsServletException*/

private void update(HttpServletRequest req, HttpServletResponse resp) throwsIOException, ServletException{

req.setCharacterEncoding("utf-8");int id = Integer.parseInt(req.getParameter("id"));

String name= req.getParameter("name");

String teacher= req.getParameter("teacher");

String classroom= req.getParameter("classroom");

Course course= newCourse(id, name, teacher, classroom);

service.update(course);

req.setAttribute("message", "修改成功");

req.getRequestDispatcher("CourseServlet?method=list").forward(req,resp);

}/*** 查找

*@paramreq

*@paramresp

*@throwsServletException*/

private void search(HttpServletRequest req, HttpServletResponse resp) throwsIOException, ServletException{

req.setCharacterEncoding("utf-8");

String name= req.getParameter("name");

String teacher= req.getParameter("teacher");

String classroom= req.getParameter("classroom");

List courses =service.search(name, teacher, classroom);

req.setAttribute("courses", courses);

req.getRequestDispatcher("searchlist.jsp").forward(req,resp);

}

}

//DBUtil.java

packagecom.hjf.util;importjava.sql.Connection;importjava.sql.DriverManager;importjava.sql.PreparedStatement;importjava.sql.ResultSet;importjava.sql.SQLException;importjava.sql.Statement;/*** 数据库连接工具

*@authorHu

**/

public classDBUtil {public static String db_url = "jdbc:mysql://localhost:3306/course";public static String db_user = "root";public static String db_pass = "root";public staticConnection getConn () {

Connection conn= null;try{

Class.forName("com.mysql.jdbc.Driver");//加载驱动

conn =DriverManager.getConnection(db_url, db_user, db_pass);

}catch(Exception e) {

e.printStackTrace();

}returnconn;

}/*** 关闭连接

*@paramstate

*@paramconn*/

public static voidclose (Statement state, Connection conn) {if (state != null) {try{

state.close();

}catch(SQLException e) {

e.printStackTrace();

}

}if (conn != null) {try{

conn.close();

}catch(SQLException e) {

e.printStackTrace();

}

}

}public static voidclose (ResultSet rs, Statement state, Connection conn) {if (rs != null) {try{

rs.close();

}catch(SQLException e) {

e.printStackTrace();

}

}if (state != null) {try{

state.close();

}catch(SQLException e) {

e.printStackTrace();

}

}if (conn != null) {try{

conn.close();

}catch(SQLException e) {

e.printStackTrace();

}

}

}public static void main(String[] args) throwsSQLException {

Connection conn=getConn();

PreparedStatement pstmt= null;

ResultSet rs= null;

String sql="select * from course";

pstmt=conn.prepareStatement(sql);

rs=pstmt.executeQuery();if(rs.next()){

System.out.println("空");

}else{

System.out.println("不空");

}

}

}

//add.jsp

Insert title here
课程信息录入
返回主页

课程名称
任课教师
上课地点
保   存

var name= document.getElementById("name");;

var teacher= document.getElementById("teacher");

var classroom= document.getElementById("classroom");//非空

if(name.value == '') {

alert('课程名称为空');

name.focus();return false;

}if(teacher.value == '') {

alert('教师为空');

teacher.focus();return false;

}if(classroom.value == '') {

alert('上课地点为空');

classroom.focus();return false;

}//教师

if(teacher.value != '王建民' && teacher.value != '王辉' && teacher.value != '刘丹' && teacher.value != '刘立嘉' && teacher.value != '杨子光'){

alert('教师名称错误');return false;

}//教室

if(!/^基教/.test(classroom.value) && !/^一教/.test(classroom.value) && !/^二教/.test(classroom.value) && !/^三教/.test(classroom.value)) {

alert('上课地点错误');return false;

}

}

//del.jsp

Insert title here
课程信息删除
返回主页

课程名称
查   找

var name= document.getElementById("classname");;//非空

if(name.value == '') {

alert('课程名称为空');

name.focus();return false;

}

}

//detail.jap

Insert title here
margin-top: 20px;

}

.b{

font-size: 20px;

width: 160px;

color: white;

background-color: greenyellow;

}

.tb, td {

border: 1px solid black;

font-size: 22px;

}

课程信息删除
返回主页

课程名称	${list.classname}
任课教师	${list.teachnamr}
上课地点	${list.site}
删   除

}else{return false;

}

}

//detail2.jsp

Insert title here
课程信息修改
返回主页

课程名称
任课教师
上课地点
修   改

var name= document.getElementById("name");;

var teacher= document.getElementById("teacher");

var classroom= document.getElementById("classroom");//非空

if(name.value == '') {

alert('课程名称为空');

name.focus();return false;

}if(teacher.value == '') {

alert('教师为空');

teacher.focus();return false;

}if(classroom.value == '') {

alert('上课地点为空');

classroom.focus();return false;

}//教师

if(teacher.value != '王建民' && teacher.value != '王辉' && teacher.value != '刘丹' && teacher.value != '刘立嘉' && teacher.value != '杨子光'){

alert('教师名称错误');return false;

}//教室

if(!/^基教/.test(classroom.value) && !/^一教/.test(classroom.value) && !/^二教/.test(classroom.value) && !/^三教/.test(classroom.value)) {

alert('上课地点错误');return false;

}

}

首页
课程信息添加

课程信息修改

课程信息删除

课程信息查询

//list.jsp

Insert title here
课程信息列表
返回主页

id	课程名称	任课教师	上课地点	操作
${item.id}	${item.name}	${item.teacher}	${item.classroom}	修改
//search.jsp

Insert title here
课程信息查询
返回主页

课程名称
任课教师
上课地点
查   询

var name= document.getElementById("name");;

var teacher= document.getElementById("teacher");

var classroom= document.getElementById("classroom");//非空

if(name.value == '' && teacher.value == '' && classroom.value == '') {

alert('请填写一个条件');return false;

}

}

//searchlist.jsp

Insert title here
课程信息列表
返回主页

id	课程名称	任课教师	上课地点
${item.id}	${item.name}	${item.teacher}	${item.classroom}
————————————————
版权声明：本文为CSDN博主「weixin_39775106」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_39775106/article/details/111986183