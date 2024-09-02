package com.music.service;

import java.sql.SQLException;

import com.music.dao.FabulousDao;

public class FabulousService {

	public boolean updateFabulous(String fabulousId, String praiseNum) throws SQLException {
		FabulousDao dao = new FabulousDao();
		boolean flag = dao.updateFabulous(fabulousId,praiseNum);
		return flag;
	}

}
