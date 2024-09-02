package com.music.test;

import java.io.IOException;
import java.util.Enumeration;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class testServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
//		String canshu = request.getQueryString(); // 获取url？后面的参数
//		System.out.println(canshu);
//		System.out.println(request);
//		Enumeration<String> requests = request.getHeaderNames(); // 获取所有请求头 
//		while (requests.hasMoreElements()) {
//			String name = (String) requests.nextElement(); // key
//			String hander = request.getHeader(name); // value
//			System.out.println(name+":"+hander);
//		}
//		String [] parameterValues = request.getParameterValues("hobby");
//		for (String string : parameterValues) {
//			System.out.println(string); // 获取的是input的value值
//		}
		// 获取所有参数，参数封装到map集合中
//		Map<String, String[]> map = request.getParameterMap();
//		for (Map.Entry<String,String[]> entry : map.entrySet()) {
//			System.out.println(entry.getKey());
//			for (String value : entry.getValue()) {
//				System.out.println(value);
//			}
//		}
		response.getWriter().append("hello world");
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}