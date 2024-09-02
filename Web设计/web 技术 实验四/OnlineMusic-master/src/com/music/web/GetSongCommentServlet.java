package com.music.web;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.Gson;
import com.music.domain.SongComment;
import com.music.service.GetSongCommentService;

public class GetSongCommentServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html;charset=utf-8");
		String sid = request.getParameter("sid");
		String uid = request.getParameter("uid");
		String method = request.getParameter("method");
		System.out.println(sid+":"+uid+":"+method);
		GetSongCommentService service = new GetSongCommentService();
		List<Map<String, Object>> list = null;
		try {
			list = service.getComment(sid,uid,method);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		Gson gson = new Gson();
		String json = gson.toJson(list);
		PrintWriter out = response.getWriter();
		out.write(json);
		out.flush();
		out.close();
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}