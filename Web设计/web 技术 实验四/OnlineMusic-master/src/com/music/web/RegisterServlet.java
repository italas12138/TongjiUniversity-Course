package com.music.web;

import java.io.IOException;
import java.io.PrintWriter;
import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.beanutils.BeanUtils;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.music.domain.User;
import com.music.service.RegisterService;

public class RegisterServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html;charset=utf-8");
		Map<String,String[]> parameterMap = request.getParameterMap();
		User user = new User();
		SimpleDateFormat Format = new SimpleDateFormat("yyyy-MM-dd");
		String date = Format.format(new Date());
		user.setRegisterDate(date);
		user.setPortrait("./images/defaultImg.jpeg");
		RegisterService service = new RegisterService();
		try {
			BeanUtils.populate(user, parameterMap); // 将数据映射到实体类
			String regMsg = service.register(user); // 插入数据库
			System.out.println(regMsg);
			Map<String,String> map=new HashMap<String,String>();  // 将数据放入map集合
			map.put("msg",regMsg);
			PrintWriter out = response.getWriter();
			Gson gson = new Gson(); // Gson 将数据转化为json格式
			String json = gson.toJson(map);
			out.println(json); // 发送给前端
			out.flush();
			out.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}