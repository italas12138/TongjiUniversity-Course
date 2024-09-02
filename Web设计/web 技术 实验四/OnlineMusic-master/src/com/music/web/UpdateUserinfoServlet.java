package com.music.web;

import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.music.service.UpdateUserinfoService;

public class UpdateUserinfoServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response)throws ServletException, IOException {
		response.setContentType("text/html;charset=utf-8");
		String uid = request.getParameter("uid");
		String attr = request.getParameter("attribute");
		String newValue = request.getParameter("newValue");
		UpdateUserinfoService service = new UpdateUserinfoService();
		String flag = null;
		try {
			flag = service.updateUserinfo(uid,attr,newValue);
		} catch (NumberFormatException | SQLException e) {
			e.printStackTrace();
		}
		response.getWriter().write(flag);
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response)throws ServletException, IOException {
		doGet(request, response);
	}

}