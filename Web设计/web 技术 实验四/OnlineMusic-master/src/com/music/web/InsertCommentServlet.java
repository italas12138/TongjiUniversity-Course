package com.music.web;

import java.io.IOException;
import java.io.PrintWriter;
import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.beanutils.BeanUtils;

import com.google.gson.Gson;
import com.music.domain.SongComment;
import com.music.service.InsertCommentService;

public class InsertCommentServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html;charset=utf-8");
		Map<String, String[]> map = request.getParameterMap();
		SongComment comment = new SongComment();
		try {
			BeanUtils.populate(comment, map);
		} catch (IllegalAccessException | InvocationTargetException e) {
			e.printStackTrace();
		}
        System.out.println(comment);
		InsertCommentService service = new InsertCommentService();
		boolean flag = false;
		try {
			flag = service.insertComment(comment);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		Map<String,Boolean> map1=new HashMap<String,Boolean>();  // 将数据放入map集合
		map1.put("flag",flag);
		PrintWriter out = response.getWriter();
		Gson gson = new Gson(); // Gson 将数据转化为json格式
		String json = gson.toJson(map1);
		out.println(json); // 发送给前端
		out.flush();
		out.close();
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}