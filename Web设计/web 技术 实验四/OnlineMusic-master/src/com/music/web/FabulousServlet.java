package com.music.web;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.SQLException;
import java.util.HashMap;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.Gson;
import com.music.service.FabulousService;

public class FabulousServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String fabulousId = request.getParameter("fabulousId");
		String praiseNum = request.getParameter("praiseNum");
		System.out.println("fabulousId:"+fabulousId);
		System.out.println("praiseNum:"+praiseNum);
		FabulousService service = new FabulousService();
		boolean flag = false;
		try {
			flag = service.updateFabulous(fabulousId,praiseNum);
			System.out.println("返回flag:"+flag);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		HashMap<String, Boolean> map = new HashMap<String, Boolean>();
		map.put("flag", flag);
		Gson gson = new Gson();
		String json = gson.toJson(map);
		PrintWriter out = response.getWriter();
		out.write(json);
		out.flush();
		out.close();
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}