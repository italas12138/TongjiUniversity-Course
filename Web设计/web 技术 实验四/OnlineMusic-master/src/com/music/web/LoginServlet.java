package com.music.web;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.google.gson.Gson;
import com.music.domain.User;
import com.music.service.LoginService;

public class LoginServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String userName = request.getParameter("username");
		String passWord = request.getParameter("password");
		System.out.println(userName+":"+passWord);
		LoginService service = new LoginService();
		try {
			User user = service.login(userName,passWord);
			if (user != null) {
				Cookie cookie1 = new Cookie("userName", userName);
				cookie1.setMaxAge(60*60*24);
				Cookie cookie2 = new Cookie("passWord", passWord);
				cookie2.setMaxAge(60*60*24);
				Cookie cookie3 = new Cookie("uid",Integer.toString(user.getUid()));
				cookie3.setMaxAge(60*60*24);
				Cookie cookie4 = new Cookie("portrait",user.getPortrait());
				cookie4.setMaxAge(60*60*24);
				response.addCookie(cookie1);
				response.addCookie(cookie2);
				response.addCookie(cookie3);
				response.addCookie(cookie4);
				HttpSession session = request.getSession();
				session.setAttribute("user", userName);
			}
			Map<String,Boolean> map=new HashMap<String,Boolean>(); // 将数据放入map集合
			if (user != null) {
				map.put("flag",true);
			} else {
				map.put("flag",false);
			}
			
			PrintWriter out = response.getWriter();
			Gson gson = new Gson(); // Gson 将数据转化为json格式
			String json = gson.toJson(map);
			out.println(json); // 发送给前端
			out.flush();
			out.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}