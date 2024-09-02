package com.music.web;

import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.music.service.AddCollectionService;

public class AddCollectionServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String uid = request.getParameter("uid");
		String sid = request.getParameter("sid");
		String method = request.getParameter("method");
		AddCollectionService service = new AddCollectionService();
		String flag = "";
		try {
			flag = service.addCollection(uid,sid,method);
		} catch (NumberFormatException | SQLException e) {
			e.printStackTrace();
		}
		System.out.println(flag);
		response.getWriter().write(flag);
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}