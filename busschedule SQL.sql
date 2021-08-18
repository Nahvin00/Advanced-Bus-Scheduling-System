-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Dec 29, 2020 at 05:41 PM
-- Server version: 10.4.13-MariaDB
-- PHP Version: 7.4.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `busschedule`
--

-- --------------------------------------------------------

--
-- Table structure for table `additional_info`
--

CREATE TABLE `additional_info` (
  `current_week` int(2) NOT NULL,
  `lect_code` int(5) NOT NULL,
  `driver_code` int(5) NOT NULL,
  `admin_code` int(3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `additional_info`
--

INSERT INTO `additional_info` (`current_week`, `lect_code`, `driver_code`, `admin_code`) VALUES
(2, 12345, 54321, 11);

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

CREATE TABLE `admin` (
  `admin_id` int(10) NOT NULL,
  `admin_name` varchar(100) NOT NULL,
  `admin_phone` varchar(20) NOT NULL,
  `admin_email` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`admin_id`, `admin_name`, `admin_phone`, `admin_email`) VALUES
(10101, 'Nahvin', '01111428002', 'scnahvin@gmail.com'),
(20202, 'Arvind Singh A/L Jainnder Singh', '01354682360', 'arvinddersingh@gmail.com');

-- --------------------------------------------------------

--
-- Table structure for table `busdriver`
--

CREATE TABLE `busdriver` (
  `driver_id` int(10) NOT NULL,
  `driver_name` varchar(100) NOT NULL,
  `driver_ic` varchar(12) NOT NULL,
  `driver_phone` varchar(20) NOT NULL,
  `driver_email` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `busdriver`
--

INSERT INTO `busdriver` (`driver_id`, `driver_name`, `driver_ic`, `driver_phone`, `driver_email`) VALUES
(5000, 'Manmathan Kumar A/L Ravi Kumar', '630801051733', '01834582641', 'mkumar63@gmail.com'),
(5001, 'Muhammad Huzaini Binti Muhammad Zaid', '730202054313', '01512654784', 'muhammadhuzaini@gmail.com'),
(5002, 'Muthu Samy A/L Rama Samy', '630109051729', '01436876423', 'muthusam12@gmail.com'),
(5003, 'Abu Bakar Bin Muhamad Salim', '770505053213', '01635678432', 'abubakarr77@gmail.com'),
(5004, 'Muhammad Haikal Bin Muhammad Faiz', '690301077777', '017234286429', 'mhaikalbinfaiz@gmail.com');

-- --------------------------------------------------------

--
-- Table structure for table `data_table`
--

CREATE TABLE `data_table` (
  `data_number` int(255) NOT NULL,
  `data_week` tinyint(2) NOT NULL,
  `data_day` varchar(9) NOT NULL,
  `lect_id` int(10) DEFAULT NULL,
  `data_10am` tinyint(3) DEFAULT NULL,
  `data_11am` tinyint(3) DEFAULT NULL,
  `data_12pm` tinyint(3) DEFAULT NULL,
  `data_1pm` tinyint(3) DEFAULT NULL,
  `data_4pm` tinyint(3) DEFAULT NULL,
  `data_6pm` tinyint(3) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `data_table`
--

INSERT INTO `data_table` (`data_number`, `data_week`, `data_day`, `lect_id`, `data_10am`, `data_11am`, `data_12pm`, `data_1pm`, `data_4pm`, `data_6pm`) VALUES
(1, 1, 'MON', 1000, 0, 20, 0, 20, 40, 0),
(2, 1, 'TUE', 1000, 0, 20, 0, 20, 40, 0),
(3, 1, 'WED', 1000, 0, 0, 0, 0, 20, 20),
(4, 1, 'THU', 1000, 0, 20, 0, 20, 40, 0),
(5, 1, 'FRI', 1000, 0, 0, 40, 0, 0, 0),
(6, 1, 'MON', 1001, 0, 20, 0, 20, 40, 0),
(7, 1, 'TUE', 1001, 0, 0, 0, 0, 20, 20),
(8, 1, 'WED', 1001, 0, 20, 0, 20, 40, 0),
(9, 1, 'THU', 1001, 0, 0, 0, 0, 20, 20),
(10, 1, 'FRI', 1001, 40, 0, 40, 0, 0, 0),
(11, 1, 'MON', 1002, 0, 0, 0, 40, 0, 0),
(12, 1, 'TUE', 1002, 0, 0, 0, 0, 0, 40),
(13, 1, 'WED', 1002, 0, 40, 0, 0, 40, 0),
(14, 1, 'THU', 1002, 0, 0, 0, 0, 0, 0),
(15, 1, 'FRI', 1002, 0, 0, 40, 0, 0, 0),
(16, 1, 'MON', 1003, 0, 0, 0, 20, 40, 0),
(17, 1, 'TUE', 1003, 0, 40, 0, 0, 20, 0),
(18, 1, 'WED', 1003, 0, 20, 0, 20, 0, 0),
(19, 1, 'THU', 1003, 0, 40, 0, 0, 20, 20),
(20, 1, 'FRI', 1003, 0, 0, 0, 0, 0, 0),
(31, 1, 'MON', 1004, 0, 10, 0, 20, 30, 40),
(32, 1, 'TUE', 1004, 0, 40, 0, 30, 20, 10),
(33, 1, 'WED', 1004, 0, 0, 0, 70, 0, 0),
(34, 1, 'THU', 1004, 0, 80, 0, 0, 10, 40),
(36, 1, 'FRI', 1004, 20, 0, 40, 0, 0, 0),
(38, 2, 'MON', 1000, 0, 10, 0, 20, 30, 20),
(39, 2, 'TUE', 1000, 0, 40, 0, 30, 20, 10),
(40, 2, 'WED', 1000, 0, 0, 0, 70, 0, 0),
(41, 2, 'THU', 1000, 0, 80, 0, 0, 10, 40),
(42, 2, 'FRI', 1000, 40, 0, 40, 0, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `encryption`
--

CREATE TABLE `encryption` (
  `encryption_number` int(255) NOT NULL,
  `lect_id` int(10) DEFAULT NULL,
  `driver_id` int(10) DEFAULT NULL,
  `admin_id` int(10) DEFAULT NULL,
  `encryption_code` int(3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `encryption`
--

INSERT INTO `encryption` (`encryption_number`, `lect_id`, `driver_id`, `admin_id`, `encryption_code`) VALUES
(1, 1000, NULL, NULL, 3),
(2, 1001, NULL, NULL, 4),
(3, 1002, NULL, NULL, 7),
(4, 1003, NULL, NULL, 7),
(5, 1004, NULL, NULL, 9),
(6, NULL, 5000, NULL, 1),
(7, NULL, 5001, NULL, 8),
(8, NULL, 5002, NULL, 3),
(9, NULL, 5003, NULL, 4),
(10, NULL, 5004, NULL, 9),
(11, NULL, NULL, 10101, 2),
(12, NULL, NULL, 20202, 7);

-- --------------------------------------------------------

--
-- Table structure for table `forgotpwd_log`
--

CREATE TABLE `forgotpwd_log` (
  `log_no` int(255) NOT NULL,
  `lect_id` int(10) DEFAULT NULL,
  `driver_id` int(10) DEFAULT NULL,
  `date` date NOT NULL,
  `status` varchar(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `forgotpwd_log`
--

INSERT INTO `forgotpwd_log` (`log_no`, `lect_id`, `driver_id`, `date`, `status`) VALUES
(1, 1000, NULL, '2020-12-01', '0'),
(2, NULL, 5003, '2020-12-01', '1');

-- --------------------------------------------------------

--
-- Table structure for table `lecturer`
--

CREATE TABLE `lecturer` (
  `lect_id` int(10) NOT NULL,
  `lect_name` varchar(100) NOT NULL,
  `lect_ic` varchar(12) NOT NULL,
  `lect_faculty` varchar(6) NOT NULL,
  `lect_phone` varchar(20) NOT NULL,
  `lect_email` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `lecturer`
--

INSERT INTO `lecturer` (`lect_id`, `lect_name`, `lect_ic`, `lect_faculty`, `lect_phone`, `lect_email`) VALUES
(1000, 'Siti Kamariah Binti Abdul Rahim', '740706011776', 'FKM', '01123568376', 'sitikamariah@gmail.com'),
(1001, 'Sri Murugan A/L Rayappan', '830101051123', 'FKM', '01255348564', 'smurugan83@gmail.com'),
(1002, 'Cheong Ian Kit', '840507031772', 'FPTT', '01456784290', 'cheongkit123@gmail.com'),
(1003, 'Muhammad Abdullah Bin Muhammad Karim', '810208061777', 'FPTT', '01923567903', 'maboii81@gmail.com'),
(1004, 'Tharani Kumari A/P Perumal', '730109055222', 'FKM', '01623458769', 'ptharanikumari@gmail.com');

-- --------------------------------------------------------

--
-- Table structure for table `passwords`
--

CREATE TABLE `passwords` (
  `encryption_number` int(255) NOT NULL,
  `p_password` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `passwords`
--

INSERT INTO `passwords` (`encryption_number`, `p_password`) VALUES
(1, 'dBetf\"gN4|5)6]7M'),
(2, 'L@i3p3p/so[IsuvxpDhZ'),
(3, 'ZfhL]^L1[}OjL?W7'),
(4, 'PTnpv\"{ZU=VGt-v5u\'l)Çe'),
(5, 'ljxw~swu}5:Z;:@B'),
(6, 'q!mBb>z7swp*pFn@'),
(7, 'IkT@IGUkQQSAQB?3:0'),
(8, 'E\"D,M-X5EgDCU&Xd5-3n5C3v'),
(9, 'R\'IdZeIAVJqJmkrsh3'),
(10, 'L;[QJ/Wp\\|^A[9B_'),
(11, 'o;{`rUc8u4uyyKqZtmf:'),
(12, 'h-i|j6k[8.9w:7;l');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `additional_info`
--
ALTER TABLE `additional_info`
  ADD PRIMARY KEY (`current_week`);

--
-- Indexes for table `admin`
--
ALTER TABLE `admin`
  ADD PRIMARY KEY (`admin_id`);

--
-- Indexes for table `busdriver`
--
ALTER TABLE `busdriver`
  ADD PRIMARY KEY (`driver_id`);

--
-- Indexes for table `data_table`
--
ALTER TABLE `data_table`
  ADD PRIMARY KEY (`data_number`),
  ADD KEY `lect_id` (`lect_id`) USING BTREE;

--
-- Indexes for table `encryption`
--
ALTER TABLE `encryption`
  ADD PRIMARY KEY (`encryption_number`),
  ADD UNIQUE KEY `lect_id` (`lect_id`),
  ADD UNIQUE KEY `driver_id` (`driver_id`),
  ADD UNIQUE KEY `admin_id` (`admin_id`);

--
-- Indexes for table `forgotpwd_log`
--
ALTER TABLE `forgotpwd_log`
  ADD PRIMARY KEY (`log_no`),
  ADD UNIQUE KEY `lect_id` (`lect_id`),
  ADD UNIQUE KEY `driver_id` (`driver_id`);

--
-- Indexes for table `lecturer`
--
ALTER TABLE `lecturer`
  ADD PRIMARY KEY (`lect_id`);

--
-- Indexes for table `passwords`
--
ALTER TABLE `passwords`
  ADD PRIMARY KEY (`encryption_number`),
  ADD UNIQUE KEY `encryption_number` (`encryption_number`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `data_table`
--
ALTER TABLE `data_table`
  MODIFY `data_number` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=46;

--
-- AUTO_INCREMENT for table `forgotpwd_log`
--
ALTER TABLE `forgotpwd_log`
  MODIFY `log_no` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `data_table`
--
ALTER TABLE `data_table`
  ADD CONSTRAINT `data_table_ibfk_1` FOREIGN KEY (`lect_id`) REFERENCES `lecturer` (`lect_id`);

--
-- Constraints for table `encryption`
--
ALTER TABLE `encryption`
  ADD CONSTRAINT `encryption_ibfk_1` FOREIGN KEY (`admin_id`) REFERENCES `admin` (`admin_id`),
  ADD CONSTRAINT `encryption_ibfk_2` FOREIGN KEY (`lect_id`) REFERENCES `lecturer` (`lect_id`),
  ADD CONSTRAINT `encryption_ibfk_3` FOREIGN KEY (`driver_id`) REFERENCES `busdriver` (`driver_id`);

--
-- Constraints for table `forgotpwd_log`
--
ALTER TABLE `forgotpwd_log`
  ADD CONSTRAINT `forgotpwd_log_ibfk_1` FOREIGN KEY (`lect_id`) REFERENCES `lecturer` (`lect_id`),
  ADD CONSTRAINT `forgotpwd_log_ibfk_2` FOREIGN KEY (`driver_id`) REFERENCES `busdriver` (`driver_id`);

--
-- Constraints for table `passwords`
--
ALTER TABLE `passwords`
  ADD CONSTRAINT `passwords_ibfk_1` FOREIGN KEY (`encryption_number`) REFERENCES `encryption` (`encryption_number`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
