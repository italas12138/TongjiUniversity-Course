package com.music.web;

import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.music.service.UpdateCommentPortraitService;

public class UpdateCommentPortraitServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String uid = request.getParameter("uid");
		String portrait = request.getParameter("portrait");
		UpdateCommentPortraitService service = new UpdateCommentPortraitService();
		String flag = null;
		try {
			flag = service.updateCommentPortrait(uid,portrait);
		} catch (NumberFormatException | SQLException e) {
			e.printStackTrace();
		}
		response.getWriter().write(flag);
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}