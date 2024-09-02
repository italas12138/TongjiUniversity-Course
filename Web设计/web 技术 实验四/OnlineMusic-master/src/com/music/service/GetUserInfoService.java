package com.music.service;

import java.sql.SQLException;
import java.util.Map;

import com.music.dao.GetUserInfoDao;
import com.music.domain.User;

public class GetUserInfoService {

	public Map<String, Object> getiInfo(String userName) throws SQLException {
		GetUserInfoDao dao = new GetUserInfoDao();
		Map<String, Object> map = dao.getiInfo(userName);
		return map;
	}

}
