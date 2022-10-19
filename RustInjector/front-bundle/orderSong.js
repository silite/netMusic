import { addOnMessageCB } from './util'

let orderListShow = false

function findNativeFunction(obj, identifiers) {
  for (var key in obj) {
    var flag = true;
    for (var _i = 0, identifiers_1 = identifiers; _i < identifiers_1.length; _i++) {
      var identifier = identifiers_1[_i];
      if (!obj[key].toString().includes(identifier))
        flag = false;
    }
    if (flag)
      return key;
  }
}

function playSong(id) {
  const _playsong_ = findNativeFunction(ctl.actionManager, [".logSource=", ".actionType!==undefined){"]);
  ctl.actionManager[_playsong_]({ id, type: "4", action: "play", from: 0, href: "", data: {} });
}

function syncOrderList(orderList) {
  window.orderList = orderList
  if (orderListShow) {
    updateOrderListDom()
  }
}

function popSong(songId) {
  emitMessage({ type: 'pop', songId })
}

function receiveSync() {
  addOnMessageCB((data) => {
    try {
      const { type, orderList: strOrderList } = JSON.parse(data.data)
      if (type !== 'play') return

      const orderList = typeof(strOrderList) === 'string' ? JSON.parse(strOrderList) : strOrderList

      syncOrderList(orderList)

      if (!orderList.length) return
      const targetSongID = orderList[0].id
      playSong(targetSongID)
    }
    catch (e) {
      console.error(e)
    }
  })
}

window.addEventListener('DOMContentLoaded', () => {
  ctl.cefPlayer[cefTriggerName].onplayend.push((msg) => {
    const { playId, reason } = msg
    // 自然切歌
    if (reason !== 'end') return
    const songId = playId.split('_')[0]
    popSong(songId)
  })
})

const svgNs = 'http://www.w3.org/2000/svg'
function getPath(d) {
  const path = document.createElementNS(svgNs, 'path')
  path.setAttribute('fill', 'currentColor')
  path.setAttribute('d', d)
  return path
}

function updateOrderListDom() {
  const orderListDom = document.querySelector('#order-list')
  const orderListUl = document.querySelector('#order-list-ul')
  orderListUl && orderListUl.remove()
  const ul = document.createElement('div')
  ul.setAttribute('id', 'order-list-ul')
  ul.setAttribute('style', 'outline: none; margin: 0;')

  const getLi = () => {
    const li = document.createElement('li')
    li.setAttribute('style', 'display: flex; align-item: center;')
    li.setAttribute('class', 'order-li')
    return li
  }
  if (!window.orderList || window.orderList.length === 0) {
    const div = document.createElement('div')
    div.innerHTML = '暂无点歌歌单'
    div.setAttribute('style', 'text-align: center; margin-top: 200px;')
    ul.appendChild(div)
  } else {
    window.orderList.forEach((item, index) => {
      const songNameContainer = document.createElement('div')
      songNameContainer.setAttribute('style', 'display: inline-flex: line-height: 34px;')

      const songName = document.createElement('span')
      songName.innerHTML = `${item.name || ''}`
      songName.setAttribute('style', 'margin-left: 19px; display: inline-block; color: #333333; line-height: 34px; width: 140px; overflow: hidden; white-space: nowrap; text-overflow: ellipsis;')
      const arName = document.createElement('span')
      arName.innerHTML = item.artist ? `(${item.artist})` : ''
      arName.setAttribute('style', 'display: inline-block; width: 70px; line-height: 34px; overflow: hidden; white-space: nowrap; text-overflow: ellipsis; color: rgba(0, 0, 0, 0.2); margin-left: 2px;')

      songNameContainer.appendChild(songName)
      songNameContainer.appendChild(arName)

      const userId = document.createElement('div')
      userId.innerHTML = item.userName
      userId.setAttribute('style', 'display: inline-block; color: #333333; line-height: 34px; width: 110px; overflow: hidden; white-space: nowrap; text-overflow: ellipsis;')

      const cancelBtn = document.createElementNS(svgNs, 'svg')
      cancelBtn.setAttribute('width', 32)
      cancelBtn.setAttribute('height', 32)
      cancelBtn.setAttribute('viewBox', '0 0 512 512')
      const path = getPath("M400 145.49L366.51 112L256 222.51L145.49 112L112 145.49L222.51 256L112 366.51L145.49 400L256 289.49L366.51 400L400 366.51L289.49 256L400 145.49z")
      cancelBtn.appendChild(path)
      const cancelBtnContainer = document.createElement('div')
      cancelBtnContainer.setAttribute('style', 'transform: scale(0.5); cursor: pointer;')
      cancelBtnContainer.setAttribute('title', '移除点歌')
      cancelBtnContainer.addEventListener('click', (e) => {
        const songId = e.target.getAttribute('data-songId')
        console.log(songId, 'songId')
        popSong(songId)
      })
      cancelBtnContainer.appendChild(cancelBtn)

      const li = getLi()
      ul.appendChild(li)
      li.appendChild(songNameContainer)
      li.appendChild(userId)
      li.appendChild(cancelBtnContainer)
      cancelBtnContainer.setAttribute('data-songId', item.id)
      if (index % 2 === 1)
        li.style.background = 'rgba(0, 0, 0, 0.02)'
    })
  }

  orderListDom.appendChild(ul)
}

function renderOrderList() {
  const orderListDom = document.createElement('div')
  orderListDom.setAttribute('style', 'width: 400px; z-index: 300; background-color: white; position: absolute; top: 60px; bottom: 72px; box-shadow: 0 2px 20px 0 rgb(0 0 0 / 20%); right: 0;')
  orderListDom.setAttribute('id', 'order-list')
  const header = document.createElement('h2')
  header.setAttribute('id', 'order-list-header')
  header.innerHTML = '点播歌单'
  header.setAttribute('style', 'height: 40px; font-size: 20px; line-height: 20px; font-weight: 800; margin: 22px 0 0 20px; border-bottom: 1px solid rgba(0, 0, 0, 0.05);')
  orderListDom.appendChild(header)

  document.body.appendChild(orderListDom)

  updateOrderListDom()
}

function addOrderListBtn() {
  const div = document.createElement('div')
  const svg = document.createElementNS(svgNs, 'svg')
  svg.setAttribute('width', 24)
  svg.setAttribute('height', 24)
  svg.setAttribute('viewBox', '0 0 24 24')

  svg.appendChild(getPath('m8 6.104l11-1.117V16a3 3 0 1 1-1-2.236v-4.72l-9 .993V17a3 3 0 1 1-1-2.236v-8.66Zm1 .904V9.03l9-.993V6.094l-9 .914ZM8 17a2 2 0 1 0-4 0a2 2 0 0 0 4 0Zm10-1a2 2 0 1 0-4 0a2 2 0 0 0 4 0Z'))
  div.appendChild(svg)
  div.setAttribute('style', 'height: 24px; width: 24px; right: 230px; transform: translateY(-50%); cursor: pointer;')
  div.setAttribute('title', '点歌列表')
  div.setAttribute('id', 'order-icon')
  document.querySelector('#main-player').insertBefore(div, document.querySelector('.spk.f-vc.f-cp.j-vol'))

  div.addEventListener('click', () => {
    orderListShow = !orderListShow
    if (orderListShow) {
      renderOrderList()
    } else {
      const targetEl = document.querySelector('#order-list')
      targetEl && targetEl.remove()
    }
  })
}

window.addEventListener('load', () => {

  document.querySelector('.btnc.btnc-nxt').addEventListener('click', () => {
    try {
      if (!window.orderList.length) return
      const targetSongId = window.orderList[0].id
      if (+window.songID !== targetSongId) return

      popSong(targetSongId)
    } catch (e) {
      console.error(e)
    }
  })

  document.querySelector('.btnc.btnc-prv').addEventListener('click', () => { })

  !window.orderSongStatusCB && (window.orderSongStatusCB = [])
  window.orderSongStatusCB.push((value) => {
    emitMessage({ orderSongStatus: value })
    const orderIcon = document.querySelector('#order-icon')
    const orderList = document.querySelector('#order-list')
    if (value) {
      addOrderListBtn()
    }else {
      orderIcon && orderIcon.remove()
      orderList && orderList.remove()
    }
  })
})

export const startOrderSong = () => {
  window.addEventListener('load', () => {
    receiveSync()
    emitMessage({ type: 'init' })
  })
}
